{* Smarty *}
{include file="_header.tpl"}


{assign var=gz value=$RDB->pobierz_tabele_wynikow("SELECT * FROM (select z.id,z.miny,z.minx,z.maxy,z.maxx,IFNULL(g.color,'909090') color,g.tag,zgs.respect from fs_zones z LEFT JOIN fs_zones_gangscore zgs ON zgs.id_zone=z.id LEFT JOIN fs_gangs g ON g.id=zgs.id_gang WHERE z.active=1 ORDER BY zgs.respect DESC) s1 GROUP BY id")}

<div class='fctop20'>

{assign var=gs value=$RDB->pobierz_tabele_wynikow("select g.id,g.name,g.tag,g.color,g.respect,g.datetime_created,g.www,count(pig.id_player) czlonkow FROM fs_gangs g LEFT JOIN fs_players_in_gangs pig ON pig.id_gang=g.id GROUP BY g.id ORDER BY respect DESC;")}
{* <table class='datatable gangi ui-widget-content ui-corner-top'>
<thead>
	<tr>
		<th class='nosort'>#</th>
		<th>Tag</th>
		<th class='nosort'>Kolor</th>
		<th>Nazwa</th>
		<th>Respekt</th>
		<th>Członków</th>
		<th class='nosort'>Istnieje od</th>
		<th class='nosort'>Strona WWW</th>
	</tr>
</thead>
<tbody> *}
{counter start=1 skip=1 assign="count"}
{foreach from=$gs item=g name=fg}
{* <tr>
	<td class='id'>{$count}&nbsp;</td>
	<td class='tag' style='color: #{$g.color};'>{$g.tag}</td>
	<td class='color' style='background: #{$g.color}; width: 64px;'>&nbsp;&nbsp;</td>
	<td class='nazwa'><a href='/stats/gang/{$g.id|intval}'>{$g.name}</a></td>
	<td class='respect'>{$g.respect|intval}</td>
	<td class='czlonkow'>{$g.czlonkow|intval}</td>
	<td class='kiedy'>{$g.datetime_created|kiedy2}</td>
	<td class='www'><a href="http://{$g.www}">{$g.www}</a></td>
</tr> *}
<div id="gang" style="float: left; width: 250px; color: #2F4979;">
	<p><a href='/stats/gang/{$g.id|intval}' style="text-decoration: none; color: #2F4979;">{$count}. <font color="#{$g.color}" style="text-shadow: 0 0 1px #000000">{$g.tag} | {$g.name}</font></a></p>
	<p>L. czlonkow: <font color="#000;">{$g.czlonkow|intval}</font></p>
	<p>Istnieje od <font color="#000;">{$g.datetime_created|kiedy2}</font></p>
	<p>Respekt: <font color="#75BB08;">{$g.respect|intval}</font></p>
	<p><br/><a href="http://{$g.www}" style="text-decoration: none; color: #2F4979;">{$g.www}</a></p>
</div>
{counter}
{/foreach} 
{* </tbody>
</table> *}
</div>
<div class='fctop30 playerhouse ui-widget ui-widget-content ui-corner-top ui-corner-bottom'>
<div id='map' style="height: 400px;"></div>
<script>
	var point=new google.maps.LatLng((((-1654)*90)/3000), ((parseFloat(1601)*90)/3000));

  //Define the mapTypes we'll be using
  var mapType = new SanMapType(1, 2, function (zoom, x, y) {ldelim}
        return x == -1 && y == -1 ? "/s/i/tiles/map.outer.png" : "/s/i/tiles/map." + zoom + "." + x + "." + y + ".png";//Where the tiles are located
  {rdelim});
	
  //Initialize the map
  var sm = new SanMap('map', {ldelim}'Mapa': mapType{rdelim}, 1, point);
  
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
  
{foreach from=$gz item=z}
	createGangZone({$z.minx},{$z.miny},{$z.maxx},{$z.maxy},'{$z.color}','{$z.tag|htmlspecialchars}',{$z.respect|default:0});
{/foreach}
</script>
</div>


<br class='clear' /><br />



{include file="_footer.tpl"}