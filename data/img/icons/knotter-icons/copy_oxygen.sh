#!/bin/bash

icons='application-exit
        arrow-down
        arrow-up
        configure-toolbars
        configure
        document-close
        document-export
        document-import
        document-new
        document-open-recent
        document-open
        document-preview
        document-print-direct
        document-print-preview
        document-print
        document-properties
        document-revert
        document-save-all
        document-save-as
        document-save
        draw-eraser
        draw-line
        draw-polygon
        edit-clear-list
        edit-clear
        edit-copy
        edit-cut
        edit-delete
        edit-node
        edit-paste
        edit-redo
        edit-select-all
        edit-select
        edit-undo
        fill-color
        format-add-node
        format-break-node
        format-connect-node
        format-disconnect-node
        format-fill-color
        format-join-node
        format-remove-node
        format-stroke-color
        help-about
        help-contents
        insert-horizontal-rule
        list-add
        list-remove
        object-flip-horizontal
        object-flip-vertical
        page-zoom
        preferences-other
        preferences-system-windows-move
        preferences-system
        preferences-system-performance
        snap-orthogonal
        stroke-cap-butt
        stroke-cap-round
        stroke-cap-square
        stroke-join-bevel
        stroke-join-miter
        stroke-join-round
        tab-close-other
        tab-close
        tab-detach
        tab-duplicate
        tab-new-backgound
        tab-new
        tools-report-bug
        transform-move
        transform-rotate
        transform-scale
        view-refresh
        view-restore
        zoom-in
        zoom-original
        zoom-out
        '
output=oxygen
input=~/Downloads/oxygen-icons-4.10.2
copy_dirs='16x16
           22x22
           48x48
           64x64'
           

rm -f $output/scalable/*
for sdir in $copy_dirs
do
    if [ ! -d $output/$sdir ]
    then
        mkdir $output/$sdir
    else
        rm -f $output/$sdir/*
    fi
done

for ico in $icons
do
    echo $ico 
    
    for sdir in $copy_dirs
    do
        find $input/$sdir -name $ico.png -type f \
            -exec echo '  ' {} '>' $output/$sdir/$ico.png \; \
            -exec cp {} $output/$sdir/$ico.png \;
    done
    
    scall=`find $input/scalable -name $ico.svgz -type f  # | tail -n 1`
    
    for scal in $scall
    do
        if echo $scal | grep -vq  small
        then
            break
        fi
    done
   
    if [ -n "$scal" ]
    then
        echo '  ' $scal '>' $output/scalable/$ico.svg
        cp $scal $output/scalable/$ico.svg.gz 
        gunzip $output/scalable/$ico.svg
    fi
done