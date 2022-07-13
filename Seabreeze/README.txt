Fonctions dans Seabreeze.cpp :

---------------------------------------------------------------------
contFromNet ( Net* net )
{
  ajouter les contacts dans net au set<Contact*> _conts;
}

buildTree ( RoutingPad* rp )
{
  Construire l'arbre de rp, ça veux dire le contact trouvé dans rp sera la racine de l'arbre
}

build_from_Node ( Node* source, Segment* seg )
{
  Après avoir crée le premier node / la racine dans buildTree, on va l'utiliser pour construire l'arbre.
}

build_branch ( double* R, double* C, Contact* contact )
{
  Parcourir la branche et trouver le Node suivant de l'arbre
}

Set_RC ( double* R, double* C, Contact* ct, Segment* sm )
{
  Calculer le RC de la branche ct-sm et ajouter la valeur dans R et C
}
