{* Smarty *}

{include file="_header.tpl"}

<div class="fctop20">
{assign var=rs value=$RDB->pobierz_tabele_wynikow("select id,vehicle,minPlayers,maxPlayers,nitro,descr,distance from fs_races r WHERE r.active=1;")}
<table class='datatable races ui-widget-content ui-corner-top'>
<thead>
	<tr>
		<th>Pojazd</th>
		<th>Nitro</th>
		<th>Nazwa</th>
		<th class="nosort">Uczestników</th>
		<th class="naturalsort">Długość</th>
	</tr>
</thead>
<tbody>
{foreach from=$rs item=r}
<tr>
	<td class='pojazd'><img src='/engine/fsdomy/s/veh/Vehicle_{$r.vehicle|intval}.jpg' width="68" height="41" /></td>
	<td class='nitro'>{if $r.nitro}<span class="ui-icon ui-icon-circle-check"></span>{else}&nbsp;{/if}</td>
	<td><a href='/stats/race/{$r.id|intval}'>{$r.descr|htmlspecialchars}</a></td>
	<td class='oddo'>{$r.minPlayers|intval}-{$r.maxPlayers|intval}</td>
	<td class='odleglosc'>{$r.distance|string_format:"%.1f"}km</td>
</tr>
{/foreach}
</tbody>
</table>

</div>






{include file="_footer.tpl"}