#!/bin/bash

wdir="$PWD"

lstImg=""
for ii in `ls -1 $wdir/z20/*.tif | sort -n`
do
    suff=`basename $ii | cut -d\_ -f3`
    pathRel="%s/%s_%s_${suff}"
    #
    ginfo=`gdalinfo $ii`
    origin=`echo "$ginfo" | grep 'Origin'`
    originX=`echo $origin | cut -d\( -f2 | cut -d\, -f1`
    originY=`echo $origin | cut -d\( -f2 | cut -d\, -f2 | cut -d\) -f1`
    #
    cornUL=`echo "$ginfo" | grep 'Upper Left'`
    cornULX=`echo $cornUL | cut -d\( -f2 | cut -d, -f1`
    cornULY=`echo $cornUL | cut -d\( -f2 | cut -d, -f2 | cut -d\) -f1`
    #
    cornLR=`echo "$ginfo" | grep 'Lower Right'`
    cornLRX=`echo $cornLR | cut -d\( -f2 | cut -d, -f1`
    cornLRY=`echo $cornLR | cut -d\( -f2 | cut -d, -f2 | cut -d\) -f1`
    #
    originBL=`echo "$originX $originY" | cs2cs +proj=merc +init=epsg:3785 -f "%0.12f"`
    originLon=`echo $originBL | awk '{ print $1 }'`
    originLat=`echo $originBL | awk '{ print $2 }'`
    #
    echo "$suff --> $originX/$originY ($originLon,$originLat), UL=[$cornULX,$cornULY], LR=[$cornLRX,$cornLRY]"
    #
    fmeta="meta_${suff}.txt"
    if [ -z "${lstImg}" ];then
        lstImg="${fmeta}"
    else
	lstImg="${lstImg}\n${fmeta}"
    fi
    fout="${wdir}/${fmeta}"
    echo "[meta]
originX=${originX}
originY=${originY}
originLon=${originLon}
originLat=${originLat}
cornUL_X=${cornULX}
cornUL_Y=${cornULY}
cornLR_X=${cornLRX}
cornLR_Y=${cornLRY}
pathRelative=${pathRel}
" > $fout
done

foutCfg="${wdir}/info.cfg"
echo "[config]
prefix=bing
scales=z18,z19,z20
loadScale=z18

objTypes=Airplane,DontCare
types/Airplane=Basic,Type1,Type2,Type3,Type4
types/DontCare=Unknown

[lstmeta]" > $foutCfg

cnt=1
for ii in `echo -e "${lstImg}"`
do
    echo "fmeta${cnt}=$ii" >> $foutCfg
    ((cnt++))
done
