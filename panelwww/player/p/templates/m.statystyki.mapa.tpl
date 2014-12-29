{* Smarty *}

{include file="_header.tpl"}

{assign var=gz value=$RDB->pobierz_tabele_wynikow("SELECT * FROM (select z.id,z.miny,z.minx,z.maxy,z.maxx,IFNULL(g.color,'909090') color,g.tag,zgs.respect from fs_zones z LEFT JOIN fs_zones_gangscore zgs ON zgs.id_zone=z.id LEFT JOIN fs_gangs g ON g.id=zgs.id_gang WHERE z.active=1 ORDER BY zgs.respect DESC) s1 GROUP BY id")}
{assign var=m value=$RDB->pobierz_tabele_wynikow("SELECT mp.mapicon,mp.pX,mp.pY,mp.opis FROM `fs_mapicons` mp")}
{assign var=do value=$RDB->pobierz_tabele_wynikow("SELECT datediff(paidTo,NOW()) dni,d.id,d.X,d.Y,d.ownerid,p.nick ownernick FROM `fs_houses` d LEFT JOIN fs_players p ON p.id=d.ownerid")}
    
<div class='fctop20 playerhouse ui-widget ui-widget-content ui-corner-top ui-corner-bottom'>
<div id='map' style="height: 800px;"></div>

<script>
  // helper ported from old fsmap
  function createMapMarker16(point, type) 
  {ldelim}
    var marker = new google.maps.Marker({ldelim}
      position: point,
      map: sm.map,
      icon: 'http://fs.i32.pl/s/i/icons/Icon_'+type+'.gif'
    {rdelim});
    return marker;
  {rdelim}
  
  //Define the mapTypes we'll be using
  var mapType = new SanMapType(1, 3, function (zoom, x, y) {ldelim}
        return x == -1 && y == -1 ? "/s/i/tiles/map.outer.png" : "/s/i/tiles/map." + zoom + "." + x + "." + y + ".png";//Where the tiles are located
  {rdelim});
	
  //Initialize the map
  var sm = new SanMap('map', {ldelim}'Mapa': mapType{rdelim}, 1);
  
  // polygon helper
  function createGangZone(miny,minx,maxy,maxx,color,gang,respekt) {ldelim}
    var latOffset = (((maxy-miny)*90)/3000);
    var lonOffset = (((maxx-minx)*90)/3000);
    var lat= (((miny)*90)/3000);
    var lon= (((minx)*90)/3000);
  
    var ourZone = new google.maps.Polygon({
      paths: [
        new google.maps.LatLng(lat, lon),
        new google.maps.LatLng(lat + latOffset, lon),
        new google.maps.LatLng(lat + latOffset, lon + lonOffset),
        new google.maps.LatLng(lat , lon + lonOffset),
      ],
      strokeColor: '#'+color,
      strokeWeight: 2,
      strokeOpacity: 0.2,
      fillColor: '#'+color,
      fillOpacity: 0.5
    });

    ourZone.setMap(sm.map);
  {rdelim}
	
	//dane z fs_domy
{foreach from=$do item=d}
  var homeInfoWindow{$d.id} = new google.maps.InfoWindow({ldelim}
    {if $d.ownerid>0 && $d.dni>=0}
      content: "<font color='black'><a href='http://fs.i32.pl/fsdomy/{$d.id}' style='color: black'><u>Dom</u></a> gracza <a href='http://fs.i32.pl/stats/player/"+{$d.ownerid}+"' style='color: black'><u><b>{$d.ownernick}</b></u></a></font>"
    {else}
      content: "<a href='http://fs.i32.pl/fsdomy/{$d.id}'><font color='black'><u> >> <b> Dom do wynajęcia</b> <<</u></font></a>"
    {/if}
	{rdelim});
	var homeMarker{$d.id} = new google.maps.Marker({ldelim}
		position: SanMap.getLatLngFromPos({$d.X}, {$d.Y}),
		map: sm.map,
    {if $d.ownerid>0 && $d.dni>=0}
      icon: 'http://fs.i32.pl/s/i/icons/Icon_32.gif'
    {else}
      icon: 'http://fs.i32.pl/s/i/icons/Icon_31.gif'
    {/if}
	{rdelim});
	google.maps.event.addListener(homeMarker{$d.id}, 'click', function() {ldelim}
		sm.map.setCenter(homeMarker{$d.id}.position);
		homeInfoWindow{$d.id}.open(sm.map,homeMarker{$d.id});
	{rdelim});
{/foreach}
	//dane z fs_mapicons
{foreach from=$m item=mp}
  var point = new SanMap.getLatLngFromPos({$mp.pX}, {$mp.pY});
	createMapMarker16(point, {$mp.mapicon});
{/foreach}
//dane z fs_zones_gangscore i fs_zones
{foreach from=$gz item=z}
	createGangZone({$z.minx},{$z.miny},{$z.maxx},{$z.maxy},'{$z.color}','{$z.tag|htmlspecialchars}',{$z.respect|default:0});
{/foreach}
	//dane z tiry.p
	var point=new google.maps.LatLng((((1508.92 )*90)/3000), ((parseFloat(-302.51)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-556.37 )*90)/3000), ((parseFloat(-490.30)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-2222.40 )*90)/3000), ((parseFloat(2228.13)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-324.31 )*90)/3000), ((parseFloat(-170.75)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((912.81 )*90)/3000), ((parseFloat(2791.18)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((463.64 )*90)/3000), ((parseFloat(-1530.28)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-88.05 )*90)/3000), ((parseFloat(-2126.96)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-1034.48 )*90)/3000), ((parseFloat(-1987.60)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((2238.66 )*90)/3000), ((parseFloat(-2465.56)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-2436.49 )*90)/3000), ((parseFloat(-1967.44)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-2426.39 )*90)/3000), ((parseFloat(2597.02)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((1670.92 )*90)/3000), ((parseFloat(1545.91)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-2284.84 )*90)/3000), ((parseFloat(2072.34)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-93.24 )*90)/3000), ((parseFloat(-1240.57)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-1208.38 )*90)/3000), ((parseFloat(848.88)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((-1662.45 )*90)/3000), ((parseFloat(-1005.66)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((2740.72 )*90)/3000), ((parseFloat(2370.08)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((1946.88 )*90)/3000), ((parseFloat(1139.08)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((2115.72 )*90)/3000), ((parseFloat(1041.67)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((718.51 )*90)/3000), ((parseFloat(1635.37)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((2613.79 )*90)/3000), ((parseFloat(2867.67)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((2824.22 )*90)/3000), ((parseFloat(1454.70)*90)/3000));
	createMapMarker16(point, 51);
	var point=new google.maps.LatLng((((1587.95)*90)/3000), ((parseFloat(-754.21)*90)/3000));
	createMapMarker16(point, 51);
	
	//dane z jetmax.p
	var point=new google.maps.LatLng((((-2306.20 )*90)/3000), ((parseFloat( -1518.91)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-1442.11 )*90)/3000), ((parseFloat( -1886.58)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-894.26 )*90)/3000), ((parseFloat( -2534.8)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-2182.99 )*90)/3000), ((parseFloat( -2712.26)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-2868.95 )*90)/3000), ((parseFloat( -2367.8)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-2890.29 )*90)/3000), ((parseFloat( -1053.74)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-2431.97 )*90)/3000), ((parseFloat( 1.27)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-1847.93 )*90)/3000), ((parseFloat( -245.57)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-1106.27 )*90)/3000), ((parseFloat( 24.63)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-545.39 )*90)/3000), ((parseFloat( -107.72)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-131.8 )*90)/3000), ((parseFloat( 904.51)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-109.53 )*90)/3000), ((parseFloat( 2102.46)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((517.03 )*90)/3000), ((parseFloat( 2291.86)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((365.98 )*90)/3000), ((parseFloat( 511.32)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((217.88 )*90)/3000), ((parseFloat( -797.2)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-209.01 )*90)/3000), ((parseFloat( -1073.1)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-470.07 )*90)/3000), ((parseFloat( -333.92)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-1144.85 )*90)/3000), ((parseFloat( 71.92)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-1887.48 )*90)/3000), ((parseFloat( -247.44)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((-2597.95 )*90)/3000), ((parseFloat( 72.95)*90)/3000));
	createMapMarker16(point, 9);
	var point=new google.maps.LatLng((((2895.04)*90)/3000), ((parseFloat( -1018.09)*90)/3000));
	createMapMarker16(point, 9);
	
	//dane z latanie.p
	var point=new google.maps.LatLng((((1394.46 )*90)/3000), ((parseFloat(1570.22)*90)/3000)); //bazy spedycyjny
	createMapMarker16(point, 5);
	var point=new google.maps.LatLng((((-180.9501 )*90)/3000), ((parseFloat(-1467.3226)*90)/3000));
	createMapMarker16(point, 5);
	var point=new google.maps.LatLng((((-2444.0566)*90)/3000), ((parseFloat(1796.9084)*90)/3000));
	createMapMarker16(point, 5);
	var point=new google.maps.LatLng((((1044.0 )*90)/3000), ((parseFloat(2123.0)*90)/3000)); //cele lotnicze
	createMapMarker16(point, 57);
	var point=new google.maps.LatLng((((2760.3 )*90)/3000), ((parseFloat(2601.3)*90)/3000));
	createMapMarker16(point, 57);
	var point=new google.maps.LatLng((((-1666.3 )*90)/3000), ((parseFloat(2507.0)*90)/3000));
	createMapMarker16(point, 57);
	var point=new google.maps.LatLng((((1779.3 )*90)/3000), ((parseFloat(110.2)*90)/3000));
	createMapMarker16(point, 57);
	var point=new google.maps.LatLng((((-1961.57 )*90)/3000), ((parseFloat(-744.33)*90)/3000));
	createMapMarker16(point, 57);
	var point=new google.maps.LatLng((((894.23)*90)/3000), ((parseFloat(-1949.29)*90)/3000));
	createMapMarker16(point, 57);
</script>
</div>
{include file="_footer.tpl"}