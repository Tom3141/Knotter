/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef KNOTVIEW_HPP
#define KNOTVIEW_HPP

#include <QGraphicsView>
#include "node.hpp"
#include "edge.hpp"
#include <QUndoStack>
#include "knotgraph.hpp"
#include "snapping_grid.hpp"

/**
    \brief Area used to manipulate and render the Knot
*/
class KnotView : public QGraphicsView
{
        Q_OBJECT

    protected:
        /// Editing modes, changing this will alter the avaliable mouse actions
        enum mode_type {
                EDIT_NODE_EDGE,     ///< Alter nodes and edges (move etc)
                INSERT_EDGE_CHAIN,  ///< Insert a chain of nodes and edges
                MOVE_GRID           ///< Move grid origin
        };

        /// Current mouse action
        enum mouse_status_type {
                DEFAULT, ///< default, mouse actions depend on mode_type
                MOVING,  ///< moving existing nodes around
                DRAGGING,///< dragging the view
                PLACING, ///< placing new items
                SELECTING,///< dragging the rubberband
                EDGING   ///< toggling an edge
        };


        mode_type mode;                 ///< Editing mode
        Node* last_node;                ///< Last meaningful node or NULL
        mouse_status_type mouse_status; ///< What the mouse is doing (make mouse events stateful)
        QPointF oldpos;                 ///< Previous position in moving actions
        QPointF startpos;               ///< Starting position in moving actions
        node_list node_chain;           ///< List of nodes used in INSERT_EDGE_CHAIN
        QGraphicsLineItem* guide;       ///< Tiny line showing the edge being edited
        QGraphicsRectItem* rubberband;  ///< Draggable selection rectangle
        QUndoStack undo_stack;          ///< Actions perfomed by the view
        KnotGraph knot;                 ///< Knot logic and graphics item
        snapping_grid grid;             ///< Grid setup

    public:
        KnotView(QWidget *parent);


    /**
        \defgroup do_command  Alter graph
        These functions do exactly what they say
        \see request_command
        @{
    */

        /// Add a node
        void do_add_node ( Node* node, node_list adjl );
        /// Remove a node
        void do_remove_node ( Node* node );
        /// Move a node
        void do_move_node ( Node* node, QPointF pos );
        /// Create an edge between a and b
        void link ( Node*a, Node*b );
        /// Remove the edge between a and b
        void unlink ( Node*a, Node*b );
        /// Change the edge type between a and b
        void do_toggle_edge ( Node*a, Node*b, Edge::type_type type );

    /// @}

        const QUndoStack& get_undo_stack() const { return undo_stack; }
        QUndoStack& get_undo_stack() { return undo_stack; }

        /// Remove all nodes and edges
        void clear();

        /// Output knot as XML
        void writeXML( QIODevice* device ) const ;
        /// Add sub-knot from XML
        bool readXML( QIODevice* device );
        /// Draw the knot to device
        void paint_knot ( QPaintDevice* device, QRectF area, bool minimal );
        /// Draw the knot
        void paint_knot ( QPainter* painter, QRectF area, bool minimal );

    /**
         \defgroup knotview_style  Style setup
         @{
    */

        void set_width ( double w );
        double get_width ( ) const;

        void set_pen ( QPen p );
        QPen get_pen() const;

        void set_brush ( QBrush b );
        QBrush get_brush ( ) const;

        knot_curve_styler::style_id get_curve_style() const;
        void set_curve_style(knot_curve_styler::style_id);

        double get_cusp_angle() const;
        void set_cusp_angle(double ca);

        double get_handle_length() const;
        void set_handle_length(double hl);

        double get_crossing_distance() const;
        void set_crossing_distance(double cd);

        Qt::PenJoinStyle get_join_style() const;
        void set_join_style(Qt::PenJoinStyle);

    /// @}

    /**
             \defgroup request_command  Request action
             These function create and undo command and delegate the actual
             execution to that.
             \see do_command
             @{
    */

        Node* add_node(QPointF pos,node_list adjl=node_list());
        Node *add_node_or_break_edge(QPointF p,node_list adjl=node_list());
        void add_node(Node* n);
        void remove_node(Node* node);
        void remove_node(Node* node,node_list adjl);
        void add_edge ( Node* p1, Node* p2 );
        void remove_edge ( Node* p1, Node* p2 );
        void move_nodes ( QPointF dest );
        void cycle_edge(Edge *e);
        void cycle_edge_inverted(Edge *e);
        void set_edge_type ( Edge* e, Edge::type_type type );

    /// @}

        /// get list of selected nodes
        node_list selected_nodes() const;

        /// get reference to the grid
        snapping_grid& get_grid();

    protected:
        QPointF get_mouse_point ( QMouseEvent *event );
        void snap ( QPointF &p, QMouseEvent *event );
        void mouseDoubleClickEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);
        Node* node_at ( QPointF p );
        Edge* edge_at ( QPointF p );

        void set_guide(QPointF from, QPointF to);
        void unset_guide();
        void mode_change();

    public slots:
        void mode_edit_node_edge();
        void mode_edge_chain();
        void mode_move_grid();
        void mode_moving_new();

        void erase_selected();
        void link_selected();
        void merge_selected();

        void select_all();

        void redraw(bool recalculate_node=true);

        void reset_view();
        void zoom ( double factor );
        void reset_zoom();

        void flip_horizontal();
        void flip_vertical();

        void toggle_knotline(bool visible);
        void toggle_graph ( bool visible );

    signals:
        void mouse_moved ( QPointF );
};

#endif // KNOTVIEW_HPP
