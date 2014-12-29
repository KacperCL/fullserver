{* Smarty *}

{assign var=r value=$RDB->pobierz_wyniki("select id,interior,minPlayers,maxPlayers,vehicle,descr,distance,partime from fs_drift_tracks where id=$rid;")}
{if $r.id>0} 
{assign var=rcp value=$RDB->pobierz_tabele_wynikow("select X,Y,Z from fs_drift_tracks_cp where aid=$rid order by so ASC;")}
{assign var=racename value=$r.descr|htmlspecialchars}
{addbc txt=$racename url="/stats/drift/$rid"}
{include file="_header.tpl"}

<div class="fctop20">

<div class='wyscig ui-widget ui-widget-content ui-corner-top ui-corner-bottom fl49'>
	<img src='/engine/fsdomy/s/veh/Vehicle_{$r.vehicle|intval}.jpg' />
	<h1>{$r.descr|htmlspecialchars}</h1>
	<p>Odległość: <b>{$r.distance}km</b>, checkpointów: <b>{$rcp|@count}</b></p>
	<p>Par time: <b>{floor($r.partime/60)|string_format:"%02d"}m{round($r.partime%60)|string_format:"%02d"}s</b></p>
</div>

<div class='tabs fr49'>
	<ul>
		<li><a href='#najlepsze-wyniki'>Najlepsze wyniki</a></li>
		<li><a href='#najczesciej-wygrywajacy'>Najczęściej wygrywający</a></li>
	</ul>
	<div id='najlepsze-wyniki'>
	{assign var=rr value=$RDB->pobierz_tabele_wynikow("SELECT rr.ts,rr.recordtime,rr.playerid,rr.finalscore,p.nick FROM fs_drift_records rr JOIN fs_players p ON p.id=rr.playerid WHERE rr.raceid=$rid ORDER BY finalscore DESC limit 20;")}
	<table class='racesrecords ui-widget-content ui-corner-top'>
	<thead>
		<tr>
			<th>&nbsp;</th>
			<th align='center'>Punkty</th>
			<th align='center'>Czas</th>
			<th align='left'>Nick</th>
			<th align='right'>Kiedy</th>
		</tr>
	</thead>
	<tbody>
	{foreach from=$rr item=re name=fr}
	<tr>
		<td class='pozycja'>{$smarty.foreach.fr.index+1}.</td>
		<td class='czas'>{$re.finalscore}</td>
		<td class='czas'>{$re.recordtime|string_format:"%.2f"}s</td>
		<td class='nazwa'><a href='/stats/player/{$re.playerid|intval}'>{$re.nick|htmlspecialchars}</a></td>
		<td class='kiedy'>{$re.ts|kiedy}</td>
	</tr>
	{/foreach}
	</tbody>
	</table>
	</div>

	<div id='najczesciej-wygrywajacy'>
	{assign var=rr value=$RDB->pobierz_tabele_wynikow("SELECT count(rr.recordtime) wygranych,avg(rr.recordtime) czas_sredni,avg(rr.finalscore) p_srednie,p.id playerid,p.nick FROM fs_drift_records rr JOIN fs_players p ON p.id=rr.playerid WHERE rr.raceid=$rid GROUP BY p.id ORDER BY count(rr.recordtime) DESC limit 20;")}
	<table class='racesrecords ui-widget-content ui-corner-top'>
	<thead>
		<tr>
			<th>&nbsp;</th>
			<th align='center'>Ilość wygranych</th>
			<th align='center'>Czas średni</th>
			<th align='center'>Średni rekord</th>
			<th align='left'>Nick</th>
		</tr>
	</thead>
	<tbody>
	{foreach from=$rr item=re name=fr}{if $re.wygranych>1}
	<tr>
		<td class='pozycja'>{$smarty.foreach.fr.index+1}.</td>
		<td class='score'>{$re.wygranych}</td>
		<td class='czas'>{$re.czas_sredni|string_format:"%.2f"}s</td>
		<td class='czas'>{round($re.p_srednie)}</td>
		<td class='nazwa'><a href='/stats/player/{$re.playerid|intval}'>{$re.nick|htmlspecialchars}</a></td>
	</tr>
	{/if}{/foreach}
	</tbody>
	</table>
	</div>
</div>
{if $r.interior>0 || $rcp[0].X<-4000 || $rcp[0].X>4000 || $rcp[0].Y<-4000 || $rcp[0].Y>4000}{*brak mapy - poza standardowa mapa sa*}{else}
<div class='fl49 racemap ui-widget ui-widget-content ui-corner-top ui-corner-bottom'>
<div id='racemap'></div>
</div>
<script>
var rcp=[
{foreach from=$rcp item=cp}
	[ {$cp.X}, {$cp.Y} ],
{/foreach}
];
{literal}

$(document).ready(function(){
	FSMapLoad('racemap');

//				markersUrl[i]='/fsdomy/'+$(this).attr('hid');


	for (var i=0;i<rcp.length;i++) {
		var point=new GLatLng((((rcp[i][1])*90)/3000), ((parseFloat(rcp[i][0])*90)/1500));
		if (i==0) map.setCenter(point, 3);
		if (i+1<rcp.length)
			map.addOverlay(createMapMarker(point, 'dot.png'));
		else
			map.addOverlay(createMapMarker16(point, 'flag_finish.png'));
	}

	var polyline = new GPolyline([
{/literal}
{foreach from=$rcp item=cp}
	new GLatLng(((({$cp.Y})*90)/3000), ((parseFloat({$cp.X})*90)/1500)),
{/foreach}
{literal}
	], "#3333ff", 10);
	map.addOverlay(polyline);


	

	

});
</script>{/literal}
{/if}

</div>
<br class='clear' />

{include file="_footer.tpl"}
{/if}