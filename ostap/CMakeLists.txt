
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py) 

macro(find_python_module module)
	string(TOUPPER ${module} module_upper)
	if(NOT PY_${module_upper})
		if    (ARGC GREATER 1 AND ARGV1 STREQUAL "REQUIRED")
			set(PY_${module}_FIND_REQUIRED TRUE)
		elseif(ARGC GREATER 2 AND ARGV2 STREQUAL "REQUIRED")
			set(PY_${module}_FIND_REQUIRED TRUE)
		endif()
		if    (ARGC GREATER 1 AND ARGV1 STREQUAL "QUIET")
			set(PY_${module}_FIND_QUIETLY TRUE)
		elseif(ARGC GREATER 2 AND ARGV2 STREQUAL "QUIET")
			set(PY_${module}_FIND_QUIETLY TRUE)
		endif()
		# A module's location is usually a directory, but for binary modules
		# it's a .so file.
		execute_process(COMMAND "${PYTHON_EXECUTABLE}" -c 
			"import re, ${module}; print(re.compile('/__init__.py.*').sub('',${module}.__file__))"
			RESULT_VARIABLE _${module}_status 
			OUTPUT_VARIABLE _${module}_location
			ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
		if(NOT _${module}_status)
			set(PY_${module_upper} ${_${module}_location} CACHE STRING 
				"Location of Python module ${module}")
		endif(NOT _${module}_status)
	endif(NOT PY_${module_upper})
	find_package_handle_standard_args(PY_${module} DEFAULT_MSG PY_${module_upper} )
	## find_package_handle_standard_args(PY_${module}   REQUIRED_VARS PY_${module_upper} )
endmacro(find_python_module)


find_python_module ( numpy QUIET )
if ( NOT ${PY_numpy_FOUND} )
message ( "----> warning (optional)          numpy is NOT found!" ) 
endif()

find_python_module ( scipy QUIET )
if ( NOT ${PY_scipy_FOUND} )
message ( "----> warning (optional)          scipy is NOT found!" ) 
endif()

if  ( ${PYTHON_VERSION} VERSION_LESS "3.6" )
## Check for PATHOS & friends

find_python_module ( pathos        QUIET )
find_python_module ( ppft          QUIET )
find_python_module ( multiprocess  QUIET )
find_python_module ( dill          QUIET )

if ( ${PY_pathos_FOUND} AND ${PY_dill_FOUND} AND ${PY_multiprocess_FOUND} AND ${PY_ppft_FOUND} ) 

message ( "----> Full pathos (pathos,dill,multiprocess,ppft) is found" )

else()

if ( NOT ${PY_pathos_FOUND} )
message ( "----> (warning) (optional)         pathos (part of pathos) is NOT found!" ) 
endif()

if ( NOT ${PY_dill_FOUND} )
message ( "----> (warning) (optional)           dill (part of pathos) is NOT found!" ) 
endif()

if ( NOT ${PY_multiprocess_FOUND} )
message ( "----> (warning) (optional)   multiprocess (part of pathos) is NOT found!" ) 
endif()

if ( NOT ${PY_ppft_FOUND} )
message ( "----> (warning) (optional)           ppft (part of pathos) is NOT found!" ) 
endif()

endif()

endif ( ${PYTHON_VERSION} VERSION_LESS "3.6" )

find_python_module ( terminaltables QUIET )
if ( NOT ${PY_terminaltables_FOUND} )
message ( "----> (warning) (optional) terminaltables is NOT found!" ) 
endif()

find_python_module ( more_itertools QUIET )
if ( NOT ${PY_more_itertools_FOUND} )
message ( "----> (warning) (optional) more_itertools is NOT found!" ) 
endif()

find_python_module ( psutil QUIET )
if ( NOT ${PY_more_psutil_FOUND} )
message ( "----> (warning) (optional) psutils        is NOT found!" ) 
endif()

if  ( ${PYTHON_VERSION} VERSION_GREATER_EQUAL "3.3" )
find_python_module ( bsddb3 QUIET )
if ( NOT ${PY_bsddb3_FOUND} )
message ( "----> (warning) (optional)         bsddb3 is NOT found!" ) 
endif()
endif()


execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "if True:
  from distutils import sysconfig as sc
  print(sc.get_python_lib(prefix='', plat_specific=True))"
  OUTPUT_VARIABLE OSTAP_PYTHON_SITE_LOCAL
  OUTPUT_STRIP_TRAILING_WHITESPACE)

install(DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${OSTAP_PYTHON_SITE_LOCAL}
        FILES_MATCHING PATTERN "*.py" 
        PATTERN  "*#*" EXCLUDE )

set(OSTAP_PYTHON_SITE_DIR ${OSTAP_PYTHON_SITE_LOCAL}  PARENT_SCOPE)





## senseless? 
set( $ENV{PYTHONPATH} ${CMAKE_INSTALL_PREFIX}:$ENV{PYTHONPATH} ) 

# first we can indicate the documentation build as an option and set it to ON by default
option(RUN_TESTS "Run tests?" ON)

enable_testing() 

execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -W error -c "import math"
  OUTPUT_VARIABLE OSTAP_PYTHON_CHECK_WARNINGS
  RESULT_VARIABLE OSTAP_PYTHON_CHECK_WARNINGS_RESULT
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET)

if ( OSTAP_PYTHON_CHECK_WARNINGS_RESULT ) 
 set( OSTAP_TEST_PYTHON_WARNINGS "" ) 
else  () 
 set( OSTAP_TEST_PYTHON_WARNINGS " -W error " ) 
endif () 

if ( OSTAP_TEST_PYTHON_WARNINGS ) 
 MESSAGE ( "All tests will be executed with " ${OSTAP_TEST_PYTHON_WARNINGS} " flag"  ) 
endif() 


file(GLOB files "${CMAKE_INSTALL_PREFIX}/${OSTAP_PYTHON_SITE_LOCAL}/ostap/*/tests/test_*.py")
find_program(NOSE_EXECUTABLE NAMES nosetests)
foreach(filename ${files})
   get_filename_component(tname ${filename} NAME_WE)
   string(CONCAT tname ostap. ${tname})
   if( ${NOSE_EXECUTABLE} )
     add_test (NAME ${tname} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing/Temporary COMMAND ${NOSE_EXECUTABLE} ${filename} --process-timeout=5000 )
   else  () 
     add_test (NAME ${tname} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing/Temporary COMMAND python ${OSTAP_TEST_PYTHON_WARNINGS} ${filename} )
   endif () 
   set_tests_properties(${tname} PROPERTIES TIMEOUT 5000 ) 
endforeach() 
file(GLOB               allfiles "${CMAKE_INSTALL_PREFIX}/${OSTAP_PYTHON_SITE_LOCAL}/ostap/*/*.py")
list(SORT               allfiles)
list(REMOVE_DUPLICATES  allfiles)
list(FILTER             allfiles EXCLUDE REGEX ".*\\/__init__\\.py$")
list(FILTER             allfiles EXCLUDE REGEX ".*\\/tests/.*\\.py$")
list(FILTER             allfiles EXCLUDE REGEX ".*\\/sp_.*\\.py$")
list(FILTER             allfiles EXCLUDE REGEX ".*\\/.*#.*\\.py$")

if (Python_VERSION VERSION_LESS "3.3")
  list(FILTER             allfiles EXCLUDE REGEX ".*\\/lzshelve\\.py$")
endif() 

set (PREFIX ${CMAKE_INSTALL_PREFIX}/${OSTAP_PYTHON_SITE_LOCAL}/ )

foreach(filename ${allfiles})

    string(REPLACE FAKEfakeFAKEfake ququququ   testname  ${filename} )
    string(REPLACE ${PREFIX} ""  testname ${testname})
    string(REPLACE "\.py" ""     testname ${testname})
    string(REPLACE "/" "_"       testname ${testname})
    string(CONCAT  testname ostap.runit_  ${testname})

    add_test (NAME ${testname} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing/Temporary  COMMAND python ${OSTAP_TEST_PYTHON_WARNINGS} ${filename} )

    set_tests_properties(${testname} PROPERTIES TIMEOUT 5000 ) 

endforeach() 

install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  
          DESTINATION            .
          FILES_MATCHING PATTERN "*.py" 
          PATTERN  "*.#*" EXCLUDE )

## nosetests -w ../ostap/tools --exe --processes=16  --process-timeout=2000 -I '.*chopping.*'
