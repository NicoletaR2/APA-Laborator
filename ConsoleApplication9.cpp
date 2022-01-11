// APA lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
#include<vector>
#include<time.h>
using namespace std;


struct Muchie
{
public:
    int start;
    int end;
    int pondere;

public:

    Muchie() :start(-1), end(-1), pondere(-1)
    {

    }

    void SetareValori(int s, int e)
    {
        start = s;
        end = e;
        pondere = abs(end - start);
    }

    bool VerificareDirectOrInvers(int newStart, int newEnd)
    {
        if (start == newStart && end == newEnd) return true;
        if (start == newEnd && end == newStart) return true;

        return false;
    }

    bool VerificareValoriDefault()
    {
        return (start == -1 && end == -1);
    }
};

enum class DensitateGraf
{
    Rare = 0,
    Avr = 1,
    Dens = 2
};

enum class NumarNoduriGraf
{
    nrNod100 = 100,
    nrNod500 = 500,
    nrNod1000 = 1000
};

struct DataGraf
{
public:
    Muchie* arrayMuchii;
    int nrMuchii;
    int nrNoduri;
};

DataGraf GenerareGrafConexNeorientatPonderat(NumarNoduriGraf gp, DensitateGraf gs)
{
    Muchie* pArrayMuchii;
    int** pMuchiiIndex;
    bool* pArrayNrNoduri;
    int nrCurentDeNoduri = 0;
    int nrCurentDeMuchii = 0;
    int nrNoduriGenerate = (int)gp;
    int nrMuchiiGenerate;

    int minMuchii, maxMuchii, totalMuchii;
    float minFi = 0, maxFi = 0;

    switch (gs)
    {
    case DensitateGraf::Rare:
    {
        minFi = 0.3f;
        maxFi = 0.42f;
    }
    break;

    case DensitateGraf::Avr:
    {
        minFi = 0.42f;
        maxFi = 0.7f;
    }
    break;

    case DensitateGraf::Dens:
    {
        minFi = 0.7f;
        maxFi = 0.92f;
    }
    break;

    default:

        break;
    }

    totalMuchii = (nrNoduriGenerate * (nrNoduriGenerate - 1)) / 2;

    minMuchii = (int)(minFi * totalMuchii);
    maxMuchii = (int)(maxFi * totalMuchii);


    nrMuchiiGenerate = minMuchii;

    pArrayMuchii = new Muchie[nrMuchiiGenerate]{ };
    pMuchiiIndex = new int* [nrNoduriGenerate];

    for (int index1 = 0; index1 < nrNoduriGenerate; ++index1)
    {
        int* pArray = new int[nrNoduriGenerate];

        for (int index2 = 0; index2 < nrNoduriGenerate; ++index2)
        {
            pArray[index2] = -1;
        }

        pMuchiiIndex[index1] = pArray;
    }

    pArrayNrNoduri = new bool[nrNoduriGenerate] {false};


    bool GenerareFinisata = false;

    while (GenerareFinisata == false)
    {
        int newStart = rand() % nrNoduriGenerate;
        int newEnd = rand() % nrNoduriGenerate;

        int verificaMuchiaDirecta;
        int verificaMuchiaInversa;

        if (newStart == newEnd)
        {
            continue;
        }

        if (newStart == nrNoduriGenerate)
        {
            newStart -= 1;
        }

        if (newEnd == nrNoduriGenerate)
        {
            newEnd -= 1;
        }

        verificaMuchiaDirecta = pMuchiiIndex[newStart][newEnd];
        verificaMuchiaInversa = pMuchiiIndex[newEnd][newStart];

        if (verificaMuchiaDirecta != verificaMuchiaInversa)
        {
            throw "verificaMuchiaDirecta != verificaMuchiaInversa";
        }

        if (verificaMuchiaDirecta != -1)
        {

            Muchie verificaMuchia = pArrayMuchii[verificaMuchiaDirecta];

            if (verificaMuchia.VerificareDirectOrInvers(newStart, newEnd))
            {
                continue;
            }
            else
            {
                throw "pMuchiiIndex contine index din pArrayMuchii pentru val default gresita! ";
            }
        }

        Muchie muchieNoua;
        muchieNoua.SetareValori(newStart, newEnd);

        pMuchiiIndex[newStart][newEnd] = nrCurentDeMuchii;
        pMuchiiIndex[newEnd][newStart] = nrCurentDeMuchii;

        pArrayMuchii[nrCurentDeMuchii++] = muchieNoua;

        if (pArrayNrNoduri[muchieNoua.start] == false)
        {
            pArrayNrNoduri[newStart] = true;

            nrCurentDeNoduri += 1;
        }

        if (pArrayNrNoduri[muchieNoua.end] == false)
        {
            pArrayNrNoduri[newEnd] = true;

            nrCurentDeNoduri += 1;
        }

        GenerareFinisata = (nrCurentDeMuchii >= nrMuchiiGenerate && nrCurentDeNoduri >= nrNoduriGenerate);

    }

    DataGraf grafData;

    grafData.nrMuchii = nrCurentDeMuchii;
    grafData.nrNoduri = nrCurentDeNoduri;
    grafData.arrayMuchii = pArrayMuchii;

    return grafData;
}

bool compare(Muchie stanga, Muchie dreapta)
{

    if (stanga.pondere != dreapta.pondere)
    {
        return (stanga.pondere) < (dreapta.pondere);
    }

    if (stanga.start != dreapta.start)
    {
        return (stanga.start) < (dreapta.start);
    }

    return (stanga.end) < (dreapta.end);

}

void PrimAlg(DataGraf graphData)
{
    cout << "Algoritmul Prim: " << endl << endl;

    int nrIteratiiPrim = 0;
    int nrDeNoduri = graphData.nrNoduri;
    int nrDeMuchii = graphData.nrMuchii;
    Muchie* pArrayDeMuchii = graphData.arrayMuchii;

    bool* utilizate = new bool[nrDeNoduri] { false };
    auto graf = new vector<Muchie>[nrDeNoduri];


    Muchie inceputMuchie;
    inceputMuchie.SetareValori(pArrayDeMuchii[0].start, pArrayDeMuchii[0].end);

    for (int ix = 0; ix < nrDeMuchii; ++ix)
    {
        Muchie _muchie = pArrayDeMuchii[ix];
        graf[_muchie.start].push_back(_muchie);


        bool InceputulMaiMare = false;


        if (inceputMuchie.pondere != _muchie.pondere)
        {
            InceputulMaiMare = inceputMuchie.pondere > _muchie.pondere;
        }
        else if (inceputMuchie.start != _muchie.start)
        {
            InceputulMaiMare = inceputMuchie.start > _muchie.start;
        }
        else
        {
            InceputulMaiMare = inceputMuchie.end > _muchie.end;
        }

        if (InceputulMaiMare)
        {
            inceputMuchie.SetareValori(_muchie.start, _muchie.end);
        }
        nrIteratiiPrim += 1;

    }

    cout << "Lista de adiacenta: " << endl << endl;

    for (int index1 = 0; index1 < nrDeNoduri; ++index1)
    {
        cout << "[" << index1 << "] :  ";

        for (int index2 = 0; index2 < graf[index1].size(); ++index2)
        {

            cout << graf[index1][index2].start << "_" << graf[index1][index2].end << "  ";

        }

        cout << endl;
    }


    cout << endl << "Muchiile utilizate : " << endl;

    int pondere_cost_min = 0;
    int nr_muchii_cost_min = 0;


    auto compara = [](Muchie stanga, Muchie dreapta)
    {

        if (stanga.pondere != dreapta.pondere)
        {
            return (stanga.pondere) > (dreapta.pondere);
        }

        if (stanga.start != dreapta.start)
        {
            return (stanga.start) > (dreapta.start);
        }

        return (stanga.end) > (dreapta.end);

    };


    std::priority_queue<Muchie, std::vector<Muchie>, decltype(compara)> q(compara);

    q.push(inceputMuchie);



    while (!q.empty())
    {
        Muchie muchie_ = q.top();
        q.pop();

        if (utilizate[muchie_.end])
        {
            if (nrDeNoduri == 100)
            {
                cout << "\tNod deja vizitat: " << muchie_.start << "_" << muchie_.end << "." << muchie_.pondere << endl;
            }

            continue;
        }

        utilizate[muchie_.end] = true;
        cout << '\t' << '\t' << muchie_.start << "_" << muchie_.end << "..." << muchie_.pondere << "\t[" << muchie_.start << "] :  ";

        for (int index = 0; index < graf[muchie_.start].size(); ++index)
        {
            cout << graf[muchie_.start][index].start << "_" << graf[muchie_.start][index].end << "." << graf[muchie_.start][index].pondere << "  ";
        }
        cout << endl;

        pondere_cost_min += muchie_.pondere;
        nr_muchii_cost_min += 1;

        for (Muchie e : graf[muchie_.end])
        {
            if (!utilizate[e.end])
            {
                q.push(e);
            }
        }
    }

    cout << "PRIM : Costul arborelui partial de cost minim: " << pondere_cost_min << ", numarul de muchii: " << nr_muchii_cost_min << endl;
    cout << "Nr. de iteratii Prim: " << nrIteratiiPrim << endl;
}



void KruskalAlg(DataGraf graphData)
{
    int nrIteratiiKruskal = 0;

    int nrNod = graphData.nrNoduri;
    int nrMuchie = graphData.nrMuchii;
    Muchie* arrayDeMuchii = graphData.arrayMuchii;
    bool* visitat = new bool[nrNod] { false };
    auto graf = new vector<Muchie>();

    for (int ix = 0; ix < nrMuchie; ++ix)
    {
        graf->push_back(arrayDeMuchii[ix]);
    }

    sort(graf->begin(), graf->end(), compare);

    int cost_min_pondere = 0;
    int cost_min_nr_muchii = 0;

    cout << endl << endl << "Algoritmul Kruskal" << endl << endl;

    cout << endl << "Muchiile utilizate: " << endl;

    for (int i = 0; i < nrMuchie; i++)
    {
        Muchie muchie = graf->at(i);

        if (!visitat[muchie.start] || !visitat[muchie.end])
        {
            cost_min_pondere += muchie.pondere;
            cost_min_nr_muchii += 1;

            visitat[muchie.start] = true;
            visitat[muchie.end] = true;

            cout << '\t' << '\t' << muchie.start << "_" << muchie.end << "." << muchie.pondere << endl;

        }
        else
        {
            cout << "\tNod deja vizitat: " << muchie.start << "_" << muchie.end << "." << muchie.pondere << endl;
        }
        nrIteratiiKruskal += 1;
    }
    cout << "KRUSKAL : Costul arborelui partial de cost minim: " << cost_min_pondere << ", numarul de muchii: " << cost_min_nr_muchii << endl;
    cout << "Nr. de iteratii Kruskal: " << nrIteratiiKruskal << endl;

}

void Executa(NumarNoduriGraf gp, DensitateGraf gs)
{
    DataGraf grafData = GenerareGrafConexNeorientatPonderat(gp, gs);
    clock_t start1 = clock();
    PrimAlg(grafData);
    clock_t end1 = clock();
    double elapsed1 = double(end1 - start1) / CLOCKS_PER_SEC;
    cout << "\nTimpul de executie Prim: " << elapsed1 << " secunde" << endl;

    clock_t start2 = clock();
    KruskalAlg(grafData);
    clock_t end2 = clock();
    double elapsed2 = double(end2 - start2) / CLOCKS_PER_SEC;
    cout << "\nTimpul de executie Kruskal: " << elapsed2 << " secunde" << endl;

}

int main()
{

    NumarNoduriGraf gp100 = NumarNoduriGraf::nrNod100;
    NumarNoduriGraf gp500 = NumarNoduriGraf::nrNod500;
    NumarNoduriGraf gp1000 = NumarNoduriGraf::nrNod1000;

    DensitateGraf gsRare = DensitateGraf::Rare;
    DensitateGraf gsAvr = DensitateGraf::Avr;
    DensitateGraf gsDens = DensitateGraf::Dens;

    int optiune;

    cout << "\t\t\tMENIU" << endl;
    cout << "Alegeti una dintre optiuni: " << endl;
    cout << "1.100 noduri graf rar" << endl << "2.100 noduri graf mediu" << endl << "3.100 noduri graf dens" << endl;
    cout << endl << endl;
    cout << "4.500 noduri graf rar" << endl << "5.500 noduri graf mediu" << endl << "6.500 noduri graf dens" << endl;
    cout << endl << endl;
    cout << "7.1000 noduri graf rar" << endl << "8.1000 noduri graf mediu" << endl << "9.1000 noduri graf dens" << endl;

    cout << "Optiunea aleasa: ";
    cin >> optiune;

    switch (optiune)
    {
    case 1:
        Executa(gp100, gsRare);
        break;
    case 2:
        Executa(gp100, gsAvr);
        break;
    case 3:
        Executa(gp100, gsDens);
        break;

    case 4:
        Executa(gp500, gsRare);
        break;
    case 5:
        Executa(gp500, gsAvr);
        break;
    case 6:
        Executa(gp500, gsDens);
        break;

    case 7:
        Executa(gp1000, gsRare);
        break;
    case 8:
        Executa(gp1000, gsAvr);
        break;
    case 9:
        Executa(gp1000, gsDens);
        break;

    }
}



    // Run program: Ctrl + F5 or Debug > Start Without Debugging menu
    // Debug program: F5 or Debug > Start Debugging menu

    // Tips for Getting Started:
    //   1. Use the Solution Explorer window to add/manage files
    //   2. Use the Team Explorer window to connect to source control
    //   3. Use the Output window to see build output and other messages
    //   4. Use the Error List window to view errors
    //   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
    //   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
