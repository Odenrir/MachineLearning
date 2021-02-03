/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include <InstanceSelection/ISRT.h>
#include "Misc/Utils.h"
#include "Misc/Plot.h"
#include "Classify/kNN.h"
#include "InstanceSelection/PSR.h"
#include "InstanceSelection/PSC.h"
#include "InstanceSelection/ISRB.h"
#include "InstanceSelection/ISBM.h"
#include "InstanceSelection/LSSm.h"
#include "InstanceSelection/LSCo.h"
#include "InstanceSelection/LSBo.h"
#include "InstanceSelection/IBR.h"
#include "InstanceSelection/ENN.h"
#include "Clustering/kMeans.h"
#include "Metrics/Euclidean.h"
#include "Metrics/HEOM.h"
#include "Metrics/HVDM.h"
#include "Validation/kFoldCrossValidation.h"

#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

/*
*argv[0] = Program Name
*/

int main(int argc, char *argv[]) {
#if defined(_WIN32) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    std::string dir = "/home/aldair/Documentos/numeric/";
    std::string ext = ".arff";
    std::vector<std::string> files = {"diabetes", "iris", "liver", "sonar", "thyroid", "wine", "letter", "spam",
                                      "segment", "pendigits", "kdd_synthetic_control"};

    std::cout << "Reading dataset...\n";

    std::vector<Instance> train;
    for (const auto &f: files) {
        train = Utils::ReadARFF(dir + f + ext);
        HEOM heom;
        std::cout << "Normalizing metric...\n";
        heom.Normalize(train);
        kNN algorithm(4, heom);
        //ISRB sel(30, heom);
        //PSR sel(30, heom);
        //PSC sel(6, heom);
        //IBR sel(3, 30, 3, 3, heom);
        //LSBo sel(heom);
        //LSCo sel(heom);
        ISRT sel(0.33, heom);
        std::cout << "Starting validation...\n";
        auto xval = new kFoldCrossValidation(10, train, algorithm);
        std::vector<float> avg = xval->ValidateIS(sel, f + " - ISRT-4.txt");
        std::vector<Instance>().swap(train);
    }
    //std::string file = "/home/aldair/Documentos/sint/sint400.csv";
    //std::string file = "/home/aldair/Documentos/sint/sint10000d110c3.csv";
    //std::vector<Instance> train = Utils::ReadCSV(file);
    //std::string file = "/home/aldair/Documentos/numeric/iris.arff";
    //std::vector<Instance> train = Utils::ReadARFF(file);
    /*Plot::PlotScatter2D(train);
    HEOM heom;
    heom.Normalize(train);*/
    /*PAM clus(3, 100, 100,heom);
    auto c = clus.BuildClustering(train);
    Plot::PlotScatter2D(c);
    std::vector<Instance> tr = std::vector<Instance>(train.size());
    int count = 0;
    for (int j = 0; j < c.size(); j++) {
        for (int i = 0; i < c[j].size(); i++) {
            tr[count] = c[j][i];
            tr[count].SetClass(j);
            tr[count].SetID(count);
            auto aux = tr[count].GetDescriptor();
            aux[aux.size() - 1] = c.size();
            tr[count].SetDescriptor(aux);
            count++;
        }
    }*/
    /*ISBM sel(1, heom);
    auto select = sel.DoSelection(train);
    auto dat = sel.GetBorderRepresentatives();
    std::vector<Instance> tmp = std::vector<Instance>(select.size() + dat.size());
    int count = 0;
    auto aux = select[count].GetDescriptor();
    aux[aux.size() - 1] = aux[aux.size() - 1] + 1;
    for (int i = 0; i < select.size(); i++) {
        tmp[count] = select[i];
        tmp[count].SetDescriptor(aux);
        count++;
    }
    for (int i = 0; i < dat.size(); i++) {
        tmp[count] = dat[i];
        tmp[count].SetDescriptor(aux);
        tmp[count].SetClass(aux[aux.size() - 1] - 1);
        count++;
    }
    Plot::ScalePlotScatter2D(train, tmp);*/
    /*ISRT sel(1, heom);
    auto select = sel.DoSelection(train);
    Plot::ScalePlotScatter2D(train, select);*/
    //std::vector<Instance> train = Utils::ReadARFF("/home/aldair/Documentos/segment-challenge.arff");
    //std::vector<Instance> train = Utils::ReadARFF("/home/aldair/Documentos/numeric/kdd_synthetic_control.arff");
    //std::vector<Instance> train = Utils::ReadCSV("/home/aldair/Documentos/sint.csv");
    //std::vector<Instance> train = Utils::ReadCSV("/home/aldair/Documentos/echocardiogram.csv");
    /*for(auto ins: train) {
        std::cout<<ins.ToString();
    }*/
    /*HEOM heom;
    std::cout << "Normalizing metric...\n";
    heom.Normalize(train);
    kNN algorithm(3, heom);
    std::cout << "Starting validation...\n";*/
    //LSSm fil(heom);
    //LSBo sel(heom);
    //IBR sel(3, 30, 3, 3, heom);
    //ENN sel(heom, 3);
    //ISRB sel(1, heom);
    //ISBM sel(1, heom);
    //PSR sel(30, heom);
    //auto xval = new kFoldCrossValidation(10, train, algorithm);
    //std::vector<float> avg = xval->ValidateIS(sel);
    //kMeans pam(4, heom);
    //auto cl = pam.BuildClustering(train);
    //Plot::PlotScatter2D(train);
    //ISRB sel(5, heom);
    //auto select = sel.DoSelection(train);
    //algorithm.Learn(select);
    //auto predict = algorithm.Classify(train);
    //std::cout<<Utils::Accuraccy(train, predict);
    //Plot::ScalePlotScatter2D(train, select);
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
    kFoldCrossValidation *xval = new kFoldCrossValidation(10, train, *alg);
    double avg = xval->Validate();
    std::cout << "avg acc: " << avg << "\n";*/



    //system("pause");
}

