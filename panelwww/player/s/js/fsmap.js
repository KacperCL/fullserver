/**
The MIT License (MIT)

Copyright (c) 2014 Mateusz Cichon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
      // EuclideanProjection taken from: http://econym.googlepages.com/example_custommapflat.htm

      // ====== Create the Euclidean Projection for the flat map ======
      // == Constructor ==
      function EuclideanProjection(a){
        this.pixelsPerLonDegree=[];
        this.pixelsPerLonRadian=[];
        this.pixelOrigo=[];
        this.tileBounds=[];
        var b=256
        var c=1;
        for(var d=0;d<a;d++){
          var e=b/2;
          this.pixelsPerLonDegree.push(b/360);
          this.pixelsPerLonRadian.push(b/(2*Math.PI));
          this.pixelOrigo.push(new GPoint(e,e));
          this.tileBounds.push(c);
          b*=2;
          c*=2
        }
      }
 
      // == Attach it to the GProjection() class ==
      EuclideanProjection.prototype=new GProjection();
 
 
      // == A method for converting latitudes and longitudes to pixel coordinates == 
      EuclideanProjection.prototype.fromLatLngToPixel=function(a,b){
        var c=Math.round(this.pixelOrigo[b].x+a.lng()*this.pixelsPerLonDegree[b]);
        var d=Math.round(this.pixelOrigo[b].y+(-2*a.lat())*this.pixelsPerLonDegree[b]);
        return new GPoint(c,d)
      };

      // == a method for converting pixel coordinates to latitudes and longitudes ==
      EuclideanProjection.prototype.fromPixelToLatLng=function(a,b,c){
        var d=(a.x-this.pixelOrigo[b].x)/this.pixelsPerLonDegree[b];
        var e=-0.5*(a.y-this.pixelOrigo[b].y)/this.pixelsPerLonDegree[b];
        return new GLatLng(e,d,c)
      };

      // == a method that checks if the y value is in range, and wraps the x value ==
/*      EuclideanProjection.prototype.tileCheckRange=function(a,b,c){
        var d=this.tileBounds[b];
        if  (a.y<0 || a.y>=d || a.x<0 || a.x>=d) { // By DracoBlue: added this, to avoid repeatition 
          return false;
        }
        return true
      }*/
       // == a method that checks if the y value is in range, and wraps the x value ==
     EuclideanProjection.prototype.tileCheckRange=function(a,b,c){
         var d=this.tileBounds[b];
         if (a.y<0||a.y>=d) {
///               return false;
   	    a.y=a.y%d;
	    if(a.y<0){
	           a.y+=d;
	     }
           }
	 if(a.x<0||a.x>=d){
	    
	    a.x=a.x%d;
	    if(a.x<0){
	           a.x+=d;
	     }
	 }
	return true
  }
														       
														       

      // == a method that returns the width of the tilespace ==      
      EuclideanProjection.prototype.getWrapWidth=function(zoom) {
        return this.tileBounds[zoom]*256;
      }
	  
	  
	// Here comes the dmap specific stuff:

	var gtasaIcons = {};
	var markers = [];
	var markersText = [];
	var markersUrl = [];
	var map = null;
	var update_c = 1;
	
    function FSMapLoad(el) {
    
		if (GBrowserIsCompatible()) {
			map = new GMap2(document.getElementById(el),{backgroundColor: '#39484B'});

			var copyright = new GCopyright(1, new GLatLngBounds(new GLatLng(-180, -180), new GLatLng(180, 180)), 0, '&copy; FS');
			var copyrights = new GCopyrightCollection('');
			copyrights.addCopyright(copyright);
			var tilelayer = new GTileLayer(copyrights, 2, 4);
			tilelayer.getTileUrl = function(tile, zoom) { 
			return '/s/i/map/'+tile.x+'x'+tile.y+'-'+(6-zoom)+".jpg"; };
			var CUSTOM_MAP = new GMapType( [tilelayer], new EuclideanProjection(40), "FullServer");
			map.addMapType(CUSTOM_MAP);
			map.setMapType(CUSTOM_MAP);
			map.addControl(new GSmallMapControl());
			map.enableScrollWheelZoom();
		}

    }

function createMapMarker(point, type) {
	
	var icon = new GIcon(); 
	icon.image = '/s/i/icons/Icon_'+type+'.gif';
	icon.iconSize = new GSize(8, 8);
	icon.iconAnchor = new GPoint(4, 4);
	icon.infoWindowAnchor = new GPoint(1, 1);
	var marker = new GMarker(point, icon);
	return marker;
}

function createMapMarker16Text(point, text, type) {
	
	if (typeof gtasaIcons[type] === "undefined") {
		var icon = new GIcon(); 
		icon.image = '/s/i/icons/Icon_'+type+'.gif';
		icon.iconSize = new GSize(16, 16);
		icon.iconAnchor = new GPoint(8, 8);
		icon.infoWindowAnchor = new GPoint(1, 1);
		gtasaIcons[type] = icon;
	}
	
	var marker = new GMarker(point, gtasaIcons[type]);
	GEvent.addListener(marker, 'click', function() {
		marker.openInfoWindowHtml(text);
	});
	return marker;
}

function createMapMarker16(point, type) {
	
	var icon = new GIcon(); 
	icon.image = '/s/i/icons/Icon_'+type+'.gif';
	icon.iconSize = new GSize(16, 16);
	icon.iconAnchor = new GPoint(8, 8);
	icon.infoWindowAnchor = new GPoint(1, 1);
	var marker = new GMarker(point, icon);
	return marker;
}

var marker;    
$(document).unload(function(){ GUnload(); });
    
function createGangZone(miny,minx,maxy,maxx,color,gang,respekt) {
	// (((-2054)*90)/3000)
  var latOffset = (((maxy-miny)*90)/3000);
  var lonOffset = (((maxx-minx)*90)/1500);
  var lat= (((miny)*90)/3000);
  var lon= (((minx)*90)/1500);
  var polygon = new GPolygon([
    new GLatLng(lat, lon ),
    new GLatLng(lat + latOffset, lon),
    new GLatLng(lat + latOffset, lon + lonOffset),
    new GLatLng(lat , lon + lonOffset),
    new GLatLng(lat, lon )
  ], "#"+color, 2, 0.2, "#"+color, 0.5);

	if (respekt>0) {
		var center=new GLatLng(lat+(latOffset/2), lon+(lonOffset/2) );
		var html="<div style='color: black;'>Gang <b>"+gang+"</b><br />Respekt na dzielnicy: "+respekt+"</div>";
		GEvent.addListener(polygon, "click", function() {
			 map.openInfoWindowHtml(center,html);
		  });
	};

  map.addOverlay(polygon);
}