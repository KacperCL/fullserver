
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
	
/*	function fetchData() {
		id=0;
		var point=new GLatLng(((parseFloat(house_y)*90)/3000),
					((parseFloat(house_x)*90)/1500));
		GDownloadUrl("fetch_data.php?uc="+update_c+"&json=", function(data) {
			update_c++;
			data = eval("("+data+")");
			if (typeof data.items !== "undefined") {
				for (id in data.items) {
					var item = data.items[id];
					var point = new GLatLng(((parseFloat(item.pos.y)*90)/3000),
								((parseFloat(item.pos.x)*90)/1500));
					if (typeof markers[item.id] === "undefined" || markers[item.id] === null) {
						markers[item.id] = {}
						var marker = createMapMarker(point, item.id, item.text, parseInt(item.icon));
						markers[item.id].marker = marker;
						markers[item.id].id = item.id;
						markersText[item.id] = "<b>" + item.name + "</b> <br/>" + item.text;
						map.addOverlay(markers[item.id].marker);
					} else {
						// already exists: update!
						markersText[item.id] = "<b>" + item.name + "</b> <br/>" + item.text;
						markers[item.id].marker.setLatLng(point);
					}
					markers[item.id].update_c = update_c;
				}
			}
			// remove the old ones
			for (i in markers) {
				if (markers[i].update_c != update_c) {
					map.removeOverlay(markers[i].marker);
					markers[i] = null;
					markersText[id] = null;
				}
			}
		});
	}*/

    function FSMapLoad() {
    
//		document.getElementById('map').style.width="512px";
//		document.getElementById('map').style.height="512px";
		

		if (GBrowserIsCompatible()) {
			map = new GMap2(document.getElementById("map"),{backgroundColor: '#39484B'});

			var copyright = new GCopyright(1, new GLatLngBounds(new GLatLng(-180, -180), new GLatLng(180, 180)), 0, '&copy; FS');
			var copyrights = new GCopyrightCollection('');
			copyrights.addCopyright(copyright);
			var tilelayer = new GTileLayer(copyrights, 2, 4);
			tilelayer.getTileUrl = function(tile, zoom) { 
			return 's/map/'+tile.x+'x'+tile.y+'-'+(6-zoom)+".jpg"; };
			var CUSTOM_MAP = new GMapType( [tilelayer], new EuclideanProjection(40), "FullServer");
			map.addMapType(CUSTOM_MAP);
			map.setMapType(CUSTOM_MAP);
			map.addControl(new GSmallMapControl());
			map.enableScrollWheelZoom();

			
//			fetchData();
//			setInterval("fetchData();",5000);
	    	    var point = new GLatLng(((parseFloat(house_y)*90)/3000), ((parseFloat(house_x)*90)/1500));

		    if (house_y!=0 && house_x!=0) {
		    	    map.setCenter(point, 3);
			    marker=createMapMarker(point, house_id, house_wolny ? 31 : 32);
			    map.addOverlay(marker);
		    } else
			    map.setCenter(point, 2);
		    if ($("table.domy>tbody>tr").length>0)  {
			var i=0;
			$("table.domy>tbody>tr").each(function(){
				var pp = new GLatLng(
						parseFloat($(this).attr('Y'))*90/3000,
						parseFloat($(this).attr('X'))*90/1500
						);
				markers[i]=createMapMarker(pp, $(this).attr('hid'),$(this).hasClass('zajety') ? 32 : 31 );
//				markersUrl[i]='/fsdomy/'+$(this).attr('hid');
				map.addOverlay(markers[i]);
				i++;
			});
		    }
		    

		}

    }

    function createMapMarker(point, id, type) {
	
	var icon = new GIcon(); 
	icon.image = 's/icons/Icon_'+type+'.gif';
	icon.iconSize = new GSize(20, 20);
	icon.iconAnchor = new GPoint(10, 10);
	icon.infoWindowAnchor = new GPoint(1, 1);
	var marker = new GMarker(point, icon);
          GEvent.addListener(marker, 'click', function() {
		document.location='/engine/fsdomy/'+id;
//            marker.openInfoWindowHtml(markersText[id]);
          });
	  GEvent.addListener(marker, 'mouseover', function() {
		$.scrollTo("tr[hid="+id+"]",1000);
		$("table.domy>tbody>tr[hid="+id+"]").addClass('hover').siblings("tr").removeClass('hover');
//		console.log(id);
          });
	  

      return marker;
    }
    
var marker;    
$(document).ready(function(){ 

    if ($("#map").length)
	FSMapLoad(); 
    $("table.domy>tbody>tr").mouseover(function(){
	var pp = new GLatLng(
                parseFloat($(this).attr('Y'))*90/3000,
                parseFloat($(this).attr('X'))*90/1500
        );
	map.panTo(pp);
																			
    });


    
}).unload(function(){ GUnload(); });
    
