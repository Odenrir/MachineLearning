#include "Utils.h"
#include "KNN.h"
#include "Euclidean.h"
#include "HEOM.h"
#include "HVDM.h"
#include "KFoldCrossValidation.h"
#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

/*
*argv[0] = Program Name
*argv[1] = Training Features
*argv[2] = Training Labels
*argv[3] = Validation Features
*argv[4] = Validation Labels
*argv[5] = Test Features
*/

int main(int argc, char* argv[])
{	
#if defined(_WIN32) && defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	std::cout<<"Reading dataset...\n";
    std::vector<Instance> train = Utils::ReadARFF("/home/aldair/Documentos/labor.arff");
    HVDM heom;
    std::cout<<"Normalizing metric...\n";
    heom.Normalize(train);
    KNN algorithm(3, heom);
    std::cout<<"Starting validation...\n";
    auto xval = new KFoldCrossValidation(10, train, algorithm);
    double avg = xval->Validate();
    std::cout << "avg acc: " << avg << "\n";
	/*std::vector<Instance> train = Utils::ReadCSVRICATIM(std::string(argv[1]));
	Utils::ReadLabelsRICATIM(std::string(argv[2]), train);
	std::vector<Instance> trainVal = Utils::ReadCSVRICATIM(std::string(argv[3]));
	Utils::ReadLabelsRICATIM(std::string(argv[4]), trainVal);
	std::vector<Instance> test = Utils::ReadCSVRICATIM(std::string(argv[5]));
	std::cout << train.size() << "\n";
	train.insert(train.end(), trainVal.begin(), trainVal.end());
	std::cout << train.size() << "\n";*/
	/*std::vector<Instance>::const_iterator it1= train.begin();
	std::vector<Instance>::const_iterator it2= train.begin() + 1000;
	std::vector<Instance> example(it1, it2);*/
	/*std::cout << "Training\n";
	RicatimAlgorithm *ricatim = new RicatimAlgorithm(20, 10, 15);
	ricatim->Learn(train);
	std::cout << "Classifying\n";
	std::vector<int> p = ricatim->Classify(test);
	std::cout << "Finished\n";
	Utils::WritePredict(p, "answer.txt");*/
	/*Classifier *alg = ricatim;		
	KFoldCrossValidation *xval = new KFoldCrossValidation(10, train, *alg);
	double avg = xval->Validate();
	std::cout << "avg acc: " << avg << "\n";*/
	

	
	//system("pause");
}

