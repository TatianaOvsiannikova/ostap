#---Create a shared library 
add_library(ostap SHARED src/format.cpp
                         src/gauss.cpp
                         src/AddBranch.cpp
                         src/AddVars.cpp
                         src/BLOB.cpp
                         src/BSpline.cpp
                         src/Bernstein.cpp
                         src/Bernstein1D.cpp
                         src/Bernstein2D.cpp
                         src/Bernstein3D.cpp
                         src/Binomial.cpp
                         src/BreitWigner.cpp
                         src/ChebyshevApproximation.cpp
                         src/Choose.cpp
                         src/Combine.cpp
                         src/Chi2Fit.cpp
                         src/Dalitz.cpp
                         src/DalitzIntegrator.cpp
                         src/DataFrameUtils.cpp
                         src/EigenSystem.cpp   
                         src/Error2Exception.cpp   
                         src/Exception.cpp
                         src/Faddeeva.cpp 
                         src/Formula.cpp   
                         src/FormulaVar.cpp   
                         src/Fourier.cpp   
                         src/Funcs.cpp   
                         src/GetWeight.cpp 
                         src/GSL_sentry.cpp 
                         src/GSL_utils.cpp 
                         src/Hesse.cpp
                         src/HistoDump.cpp
                         src/HistoInterpolation.cpp
                         src/HistoInterpolators.cpp
                         src/HistoMake.cpp
                         src/HistoProject.cpp
                         src/HistoStat.cpp
                         src/IFuncs.cpp
                         src/Integrator.cpp
                         src/Interpolation.cpp
                         src/Iterator.cpp
                         src/Kinematics.cpp
                         src/KramersKronig.cpp
                         src/Lomont.cpp
                         src/LorentzVectorWithError.cpp
                         src/Math.cpp
                         src/MatrixUtils.cpp                         
                         src/Models.cpp
                         src/Models2D.cpp
                         src/Moments.cpp
                         src/MoreMath.cpp
                         src/MoreRooFit.cpp
                         src/MoreVars.cpp
                         src/Mute.cpp
                         src/NStatEntity.cpp
                         src/Notifier.cpp
                         src/Ostap.cpp
                         src/OstapDataFrame.cpp
                         src/P2Quantile.cpp
                         src/Parameterization.cpp
                         src/Params.cpp
                         src/Peaks.cpp
                         src/PDFs.cpp
                         src/PDFs2D.cpp
                         src/PDFs3D.cpp
                         src/PhaseSpace.cpp
                         src/Piecewise.cpp
                         src/Point3DWithError.cpp
                         src/Polynomials.cpp   
                         src/Polarization.cpp
                         src/Primitives.cpp
                         src/Printable.cpp
                         src/PyBLOB.cpp
                         src/PyCallable.cpp 
                         src/PyFuncs.cpp 
                         src/PyPdf.cpp   
                         src/PyIterator.cpp
                         src/PySelector.cpp
                         src/PySelectorWithCuts.cpp
                         src/RootID.cpp
                         src/SFactor.cpp
                         src/StatEntity.cpp
                         src/StatVar.cpp
                         src/StatusCode.cpp
                         src/Tee.cpp
                         src/Tensors.cpp
                         src/Topics.cpp
                         src/Tmva.cpp
                         src/UStat.cpp
                         src/Valid.cpp
                         src/ValueWithError.cpp
                         src/Vector3DWithError.cpp
                         src/Voigt.cpp
                         src/Workspace.cpp    
                         src/WStatEntity.cpp    
                         src/nSphere.cpp      
                         src/owens.cpp      
                         src/hcubature.cpp                         
                         src/pcubature.cpp
                        )