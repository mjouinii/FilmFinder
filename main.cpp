#include <iostream>
#include <string>
#include <fstream>
using namespace std;
struct film {
    string titre;
    int annee;
    string realisateur;
    int duree;
    string langue;
};
struct ensemble {
    film * tabf;
    int nb;
};
struct selection {
    film ** tabf;
    int nb;
};
//*****************************
void lire_film (ifstream &flux , film &f) 
{
    getline(flux , f.titre);
    flux >> f.annee;
    flux.ignore();
    getline(flux,f.realisateur);
    flux >> f.duree;
    flux.ignore();
    getline(flux,f.langue);
}
bool importer_ensemble(const string &ch , ensemble &ens)
{
    ifstream flux(ch);
    if (!flux.is_open())
    {
        cerr << "ERREUR : impossible d'ouvrir le fichier " << ch <<endl;
        return false;
    }
    flux >> ens.nb;
    flux.ignore();
    ens.tabf = new film[ens.nb];
    for (int i=0 ; i<ens.nb ; ++i)
        lire_film(flux , ens.tabf[i]);
    flux.close();
    return true;
}
void affiche (const string &ch , ensemble &ens)
{
     if (importer_ensemble(ch,ens))
    {
        cout << ens.nb <<" films importes "<<endl;
        for (int i=0 ; i<ens.nb-1 ; ++i)
        {
            cout << "film numero " << i+1 <<endl;
            cout <<"******************"<<endl;
            cout << "Titre : "<<ens.tabf[i].titre<<endl;
            cout << "Annee : "<<ens.tabf[i].annee<<endl;
            cout << "Realisateur : "<<ens.tabf[i].realisateur<<endl;
            cout << "duree : "<<ens.tabf[i].duree<<endl;
            cout << "langue : "<<ens.tabf[i].langue<<endl;
            cout <<"******************"<<endl;
        }
    }
}
void selection_ensemble(ensemble &ens , selection &sel)
{
    sel.nb = ens.nb ;
    sel.tabf = new film*[sel.nb];
    for (int i=0 ; i<sel.nb ; ++i)
        sel.tabf[i] = &ens.tabf[i];
}
void saisi_criteres(string &realisateur , string &langue , int &annee_min , int &annee_max , int &duree_min , int &duree_max )
{
    cout << "Realisateur ( X pour tout ) : ";
    getline(cin , realisateur);
    cout << "Langue ( X pour tout ) : ";
    getline(cin , langue);
    cout << "Annee minimale : ";
    cin >> annee_min;
    cout << "Annee maximale : ";
    cin >> annee_max;
    cout << "Duree minimale (min) : ";
    cin >> duree_min;
    cout << "Durer maximale (min) : ";
    cin >> duree_max;
    cin.ignore();
}
bool correspand(const film &f , const string realisateur , const string langue , int annee_min , int annee_max , int duree_min , int duree_max )
{
    if (realisateur != "X" && realisateur != "x" && realisateur != f.realisateur)
        return false;
    if (langue != "X" && langue != "x" && langue  != f.langue)
        return false;
    if (f.annee < annee_min || f.annee > annee_max)
        return false;
    if (f.duree < duree_min || f.duree > duree_max )
        return false;
    return true;
}
void selection_critere (const ensemble &ens , selection &sel , const string realisateur , const string langue , int annee_min , int annee_max , int duree_min , int duree_max)
{
    sel.nb = 0;
    for (int i=0 ; i<ens.nb ; ++i)
        if (correspand(ens.tabf[i],realisateur,langue,annee_min,annee_max,duree_min,duree_max))
            sel.nb ++;
    sel.tabf = new film*[sel.nb];
    int j=0;
    for (int i=0 ; i<ens.nb ; ++i)
        if (correspand(ens.tabf[i],realisateur,langue,annee_min,annee_max,duree_min,duree_max))
        {
            sel.tabf[j] = &ens.tabf[i];
            ++j;
        }   
}
void affiche_selection(const selection &sel)
{
    for (int i=0 ; i<sel.nb ; ++i)
    {
        cout << sel.tabf[i]->titre << " (";
        cout << sel.tabf[i]->realisateur <<" ,";
        cout << sel.tabf[i]->langue <<" ,";
        cout << sel.tabf[i]->annee << ")"<<endl;
    }
}
void enregistrer(const selection &sel , string nom_fichier )
{
    ofstream flux(nom_fichier);
    if (!flux.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier "<<nom_fichier<<endl;
        return ;
    }
    flux << sel.nb << endl ;

    for (int i=0 ; i<sel.nb ; ++i)
    {
        flux << sel.tabf[i]->titre << endl ;
        flux << sel.tabf[i]->annee<< endl ;
        flux << sel.tabf[i]->realisateur << endl ;
        flux << sel.tabf[i]->duree << endl ;
        flux << sel.tabf[i]->langue << endl ;
    }
    flux.close();
}
int nb_film_commun (const selection &sel1 , const selection &sel2)
{
    int nb=0;
    for (int i=0 ; i<sel1.nb ; ++i)
        for (int j=0 ; j<sel2.nb ; ++j)
            if (sel2.tabf[j]->titre == sel1.tabf[i]->titre )
                ++nb;
    return nb;
}
void tri (selection &sel)
{
    for (int i=0 ; i<sel.nb-1 ; ++i)
        for (int j=0 ; j<sel.nb-i-1 ; ++j)
            if (sel.tabf[j]->annee > sel.tabf[j+1]->annee )
            {
                film *temp = sel.tabf[j] ;
                sel.tabf[j] = sel.tabf[j+1] ;
                sel.tabf[j+1] = temp;
            }
}
//*****************************
int main()
{
    ensemble ens;
    selection sel;
    string realisateur , langue ;
    int annee_min, annee_max, duree_min, duree_max;
    
    // Importer et afficher tous les films
        affiche("data.txt", ens);
    
    // Saisir les critères
    cout << "\n=== RECHERCHE DE FILMS ===" << endl;
    saisi_criteres(realisateur,langue , annee_min, annee_max, duree_min, duree_max);
    
    // Créer la sélection
    selection_critere(ens, sel, realisateur, langue , annee_min, annee_max, duree_min, duree_max);
    
    // Trier et afficher
    tri(sel);
    cout << "\n"<<sel.nb<<" FILMS TROUVES : " << endl;
    affiche_selection(sel);
    
    // Enregistrer
    enregistrer(sel, "selection.txt");
    cout << "\nSelection enregistree dans selection.txt" << endl;
    
    // Libérer la mémoire
    delete[] sel.tabf;
    delete[] ens.tabf;
    
    return 0;
}
