
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#ifndef __NIMBUS_GRID_H__
#define __NIMBUS_GRID_H__

#include "hurricane/Contact.h"
#include "nimbus/GCells.h"
#include "nimbus/Fences.h"

namespace Nimbus {


  using namespace Hurricane;

  class NimbusEngine;


class Grid {


    public:
        class FenceSet : public IntrusiveSet<Fence> {
            public:
                typedef IntrusiveSet<Fence> Inherit;
                FenceSet();
                virtual unsigned _getHashValue(Fence* fence) const;
                virtual Fence* _getNextElement(Fence* fence) const;
                virtual void _setNextElement(Fence* fence, Fence* nextFence) const;
        };

    private: NimbusEngine*             _nimbus;
    private: Cell*                     _cell;
    private: GCell*                    _rootGCell;
    private: Layer*                    _layer;
    private: map<unsigned, FenceSet*>  _fences;

  /*!
   *  \fn  Grid::Grid ( NimbusEngine* nimbus )
   *  \param nimbus  The engine owning the grid.
   */
    public: Grid (NimbusEngine* nimbus);
    public: ~Grid ();

    public: Cell* getCell() const { return _cell; };
    public: NimbusEngine* getNimbus() const { return _nimbus; };
   /*!
    * \fn void Grid::rectangularShape ( double margin, double aspectRatio, DbU::Unit sumWidth, DbU::Unit minWidth )
    * \param margin       Additionnal free space in the abutment box
    * \param aspectRatio
    * \param sumWidth
    */
  public: void rectangularShape(double margin, double aspectRatio, DbU::Unit sumWidth, DbU::Unit minWidth);

   /*!
    * \fn void Grid::horizontalLongSplit (unsigned step, DbU::Unit& Y)
    * \param step    the hierarchy level to be splitted
    * \param Y       ordinate of the cut-line
    * Makes a full horizontal split of the abutment box apart from a cut line
    */
    public: void horizontalLongSplit (unsigned step, DbU::Unit& Y) const;
   /*!
    * \fn void Grid::verticalLongSplit (unsigned step, DbU::Unit& X)
    * \param step    the hierarchy level to be splitted
    * \param X       abscisse of the cut-line
    * Makes a full vertical split of the abutment box apart from a cut line
    */
    public: void verticalLongSplit (unsigned step, DbU::Unit& X) const;

    public: static Grid* create (unsigned n, DbU::Unit& width, DbU::Unit& height);

            // Layer du routage global
   /*!
    * \fn Layer* Grid::getLayer ()
    * Returns the hurricane layer for the global route elements (splitters)
    */
    public: Layer* getLayer () const;

            // Fenetre racine (boite d'aboutement)
   /*!
    * \fn GCell* Grid::getRoot ()
    * Returns the root of the partitionning box tree
    */
    public: GCell* getRoot () const { return _rootGCell; };

            // Retourne la boite englobante pour p
   /*!
    * \fn GCell* Grid::getGCell (unsigned step, Point& p)
    * \param step    the level to be searched
    * \param p       the point
    * Returns the hierarchy level "step" GCell containing p
    */
    public: GCell* getGCell (unsigned step, Point& p) const;
   /*!
    * \fn Box* Grid::getBox (unsigned step, Point& p)
    * \param step    the level to be searched
    * \param p       the point
    * Returns the hierarchy level "step" hurricane Box containing p
    */
    public: const Box getBox (unsigned step, Point& p) const;

            // Centre de fenetre correspondant à X, Y, pos pour l'étape step.
    public: DbU::Unit getX (unsigned step, DbU::Unit& X) const;
    public: DbU::Unit getY (unsigned step, DbU::Unit& Y) const;
    public: Point getPosition (unsigned step, Point& pos) const;

    public: GCells getGCells (unsigned step) const;
    public: GCells getFastGCells (unsigned step) const;

    public: void createFences (unsigned step);
    public: Fences getFences (unsigned step);
    public: Fence* getFenceBetween (GCell* gc1, GCell* gc2) const;
    public: Fence* testFenceBetween (GCell* gc1, GCell* gc2) const;

    public: GCell* getLeftOf (const GCell* gcell) const;
    public: GCell* getRightOf (const GCell* gcell) const;
    public: GCell* getUpOf (const GCell* gcell) const;
    public: GCell* getDownOf (const GCell* gcell) const;

    public: GCell* getUpperLeftCorner (unsigned step) const;
    public: GCell* getUpperRightCorner (unsigned step) const;
    public: GCell* getLowerLeftCorner (unsigned step) const;
    public: GCell* getLowerRightCorner (unsigned step) const;
    
            // affichage, pour le debug
    public: void dumpGrid() const;
    private: void recDumpGrid (GCell* gcell) const;

             // utilisation interne uniquement
    private: void recLS (unsigned step, DbU::Unit& coord, unsigned direction, GCell* gcell) const;
    private: GCell* recGetGCell (unsigned step, Point& p, GCell* gcell) const;




            // compat
    public: Box getGRBox (unsigned step, Point &p);

    public: Contact* getContact (Net& net, Point& position);
  public: Contact* getOrCreateContact (Net& net, Point& position, DbU::Unit width = DbU::lambda(5), DbU::Unit height = DbU::lambda(5));
    //public: Contact* getOrCreateContact (Net& net, Point& position);

    public: DbU::Unit getHSplitterHalfLength (unsigned _step);
    public: DbU::Unit getVSplitterHalfLength (unsigned _step);

    public: Point getSubUpRight (unsigned step, Point &pos);
    public: Point getSubUpLeft (unsigned step, Point &pos);
    public: Point getSubDownRight (unsigned step, Point &pos);
    public: Point getSubDownLeft (unsigned step, Point &pos);
};

Grid* getGrid ();

}
#endif
