{* Smarty *}

{include file="_header.tpl"}

<div class="fctop20">

{assign var=rs value=$RDB->pobierz_tabele_wynikow("select id,vehicle,minPlayers,maxPlayers,nitro,descr,distance,partime from fs_drift_tracks r WHERE r.active=1;")}
<div class='fl74'>
<table class='datatable races ui-widget-content ui-corner-top'>
<thead>
	<tr>
		<th>Pojazd</th>
		<th>Nitro</th>
		<th>Nazwa</th>
		<th class="nosort">Uczestników</th>
		<th class="naturalsort">Długość</th>
		<th class="naturalsort"><acronym title="Czas przeznaczony na pokonanie toru">Par time</acronym></th>
	</tr>
</thead>
<tbody>
{foreach from=$rs item=r}
<tr>
	<td class='pojazd'><img src='/engine/fsdomy/s/veh/Vehicle_{$r.vehicle|intval}.jpg' width="68" height="41" /></td>
	<td class='nitro'>{if $r.nitro}<span class="ui-icon ui-icon-circle-check"></span>{else}&nbsp;{/if}</td>
	<td><a href='/stats/drift/{$r.id|intval}'>{$r.descr|htmlspecialchars}</a></td>
	<td class='oddo'>{$r.minPlayers|intval}-{$r.maxPlayers|intval}</td>
	<td class='odleglosc'>{$r.distance|string_format:"%.1f"}km</td>
	<td class='partime'>{floor($r.partime/60)|string_format:"%02d"}m{round($r.partime%60)|string_format:"%02d"}s</td>
</tr>
{/foreach}
</tbody>
</table>
</div>

<div class='tabs fr24'>

	<ul>
		<li><a href='#najlepsi-tydzien'>Najlepsi w tym tygodniu</a></li>
		<li><a href='#najlepsi-ever'>Najlepsi od zawsze</a></li>
	</ul>

<div id='najlepsi-tydzien'>
{assign var=bd value=$RDB->pobierz_tabele_wynikow("SELECT dr.playerid,p.nick,sum(dr.finalscore) punktow from fs_drift_records dr JOIN fs_players p ON p.id=dr.playerid WHERE datediff(NOW(),dr.ts)<=7 GROUP BY dr.playerid order by punktow desc limit 10;")}
<table class='races datatable ui-widget-content ui-corner-top'>
<thead>
 <tr>
	<th>P.</td>
	<th>Nick</td>
	<th>Punkty</th>
 </tr>
</thead>
<tbody>
{foreach from=$bd item=r name=f}
<tr>
	<td align='center'>{$smarty.foreach.f.index+1}.</td>
	<td align='left'><a href='/stats/player/{$r.playerid|intval}'>{$r.nick|htmlspecialchars}</a></td>
	<td align='right'>{round($r.punktow/1000)}k</td>
</tr>
{/foreach}
</tbody>
</table>
</div>

<div id='najlepsi-ever'>
{assign var=bd value=$RDB->pobierz_tabele_wynikow("SELECT dr.playerid,p.nick,sum(dr.finalscore) punktow from fs_drift_records dr JOIN fs_players p ON p.id=dr.playerid GROUP BY dr.playerid order by punktow desc limit 10;")}
<table class='races datatable ui-widget-content ui-corner-top'>
<thead>
 <tr>
	<th>P.</td>
	<th>Nick</td>
	<th>Punkty</th>
 </tr>
</thead>
<tbody>
{foreach from=$bd item=r name=f}
<tr>
	<td align='center'>{$smarty.foreach.f.index+1}.</td>
	<td align='left'><a href='/stats/player/{$r.playerid|intval}'>{$r.nick|htmlspecialchars}</a></td>
	<td align='right'>{round($r.punktow/1000)}k</td>
</tr>
{/foreach}
</tbody>
</table>
</div>




</div>
</div>

<br class='clear' />



{include file="_footer.tpl"}