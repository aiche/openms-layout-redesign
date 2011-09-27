### the directory name
set(directory source/APPLICATIONS/TOPP)

### list all filenames of the directory here
set(TOPP_executables
AdditiveSeries
BaselineFilter
CompNovo
CompNovoCID
ConsensusID
ConsensusMapNormalizer
DBExporter
DBImporter
DTAExtractor
Decharger
EICExtractor
ExecutePipeline
FalseDiscoveryRate
FeatureFinderMRM
FeatureFinderCentroided
FeatureFinderIsotopeWavelet
FeatureFinderMetabo
FeatureFinderRaw
FeatureLinkerLabeled
FeatureLinkerUnlabeled
FeatureLinkerUnlabeledQT
FileConverter
FileFilter
FileInfo
FileMerger
GenericWrapper
HighResPrecursorMassCorrector
IDConflictResolver
IDDecoyProbability
IDPosteriorErrorProbability
IDFileConverter
IDFilter
IDMapper
IDMerger
IDRTCalibration
ITRAQAnalyzer
InspectAdapter
InternalCalibration
MapAlignerApplyTransformation
MapAlignerIdentification
MapAlignerPoseClustering
MapAlignerSpectrum
MapNormalizer
MapStatistics
MascotAdapter
MascotAdapterOnline
MassTraceExtractor
NoiseFilterGaussian
NoiseFilterSGolay
OMSSAAdapter
PhosphoScoring
PILISIdentification
PILISModel
PTModel
PTPredict
PeakPickerHiRes
PeakPickerWavelet
PepNovoAdapter
PeptideIndexer
PrecursorIonSelector
PrecursorMassCorrector
ProteinInference
ProteinQuantifier
RTModel
RTPredict
Resampler
SILACAnalyzer
SeedListGenerator
#SequestAdapter
SpecLibSearcher
SpectraFilter
TOFCalibration
TextExporter
XTandemAdapter
InclusionExclusionListCreator
SpectraMerger
)

## all targets with need linkage against OpenMS_GUI.lib - they also need to appear in the list above)
set(TOPP_executables_with_GUIlib
ExecutePipeline
)

### add filenames to Visual Studio solution tree
set(sources_VS)
foreach(i ${TOPP_executables})
	list(APPEND sources_VS "${i}.C")
endforeach(i)
source_group("" FILES ${sources_VS})
