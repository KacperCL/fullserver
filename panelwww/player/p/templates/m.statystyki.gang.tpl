{* Smarty *}
{assign var=g value=$RDB->pobierz_wyniki("select id,name,tag,color,respect,datetime_created,www FROM fs_gangs WHERE id=$gid")}
{if $g.id>=0}
{assign var=gname value=$g.name|htmlspecialchars}
{addbc txt="$gname" url="/stats/gang/$gid"}


{include file="_header.tpl"}

<div class="fctop20">

{assign var=respekt_pozycja value=$RDB->wynik("select respect into @resp from fs_gangs where id=$gid;")}
{assign var=respekt_pozycja value=$RDB->wynik("select count(id) from fs_gangs where respect>=@resp;")}
{assign var=czlonkowie value=$RDB->pobierz_tabele_wynikow("SELECT p.id,p.nick,p.respect,p.skill,p.datetime_last,pig.rank,pig.join_date FROM fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player WHERE pig.id_gang=$gid ORDER  BY pig.rank DESC")}

<div class='gangdet ui-widget ui-widget-content ui-corner-top ui-corner-bottom cols3'>
	<h1 style='color: #{$g.color};' class='tag'>{$g.tag|htmlspecialchars}</h1>
	<h1>{$g.name}</h1>
	<div>
	<p>Respekt: <b>{$g.respect}</b> (<a href='/stats/gangs/'><i>{$respekt_pozycja}. miejsce</i></a>)</p>
	<p>Członków: {$czlonkowie|@count}</p>
	<p>Strona WWW: <a href="http://{$g.www}">{$g.www}</a></p>
	</div>
	<div>
	</div>


</div>
<br class='clear' />

<div class='tabs fr24'>
	<ul>
		<li><a href='#gang-areny'>Areny</a></li>
	</ul>
	<div id='gang-areny'>
	{assign var=gas value=$RDB->pobierz_tabele_wynikow("select a.id,a.descr,sum(pa.kills) score from fs_arena a JOIN fs_players_arenascore pa ON a.id=pa.id_arena JOIN fs_players_in_gangs pig ON pig.id_player=pa.id_player AND pig.id_gang=$gid GROUP BY a.id;")}
		<table class='datatable arenascoresmall'>
		<thead>
			<tr>
				<th>Arena</th>
				<th>Score</th>
			</tr>
		</thead>
		<tbody>
		{foreach from=$gas item=as}
			<tr>
				<td class='nazwa'><a href='/stats/areny#arena-{$as.id|intval}'>{$as.descr|htmlspecialchars}</a></td>
				<td class='kills'><a href='/stats/areny#arena-{$as.id|intval}'>{$as.score|intval}</a></td>
			</td>
		{/foreach}
		</tbody>
		</table>
	</div>
</div>

<div class='tabs fl74'>
	<ul>
		<li><a href='#gang-czlonkowie'>Członkowie</a></li>

	</ul>
	<div id='gang-czlonkowie'>

<table class='czlonkowie datatable'>
<thead>
	<tr>
		<th>Ranga</th>
		<th>Nick</th>
		<th class='naturalsort'>Respekt</th>
		<th class='naturalsort'>Skill</th>
		<th class="nosort">Członek od</th>
		<th class="nosort">Ostatnio aktywny</th>
	</tr>
</thead>
<tbody>
{foreach from=$czlonkowie item=m}
<tr>
	<td>{if $m.rank=='owner'}Założyciel{else if $m.rank=='viceowner'}Współzałożyciel{else if $m.rank=='leader'}Lider{else}&nbsp;{/if}</td>
	<td><a href='/stats/player/{$m.id|intval}'>{$m.nick|htmlspecialchars}</a></td>
	<td class='respect'>{$m.respect|intval}</td>
	<td class='skill'>{if $m.skill>0}{$m.skill|intval}{else}&nbsp;{/if}</td>
	<td class='dolaczyl'>{$m.join_date|kiedy2}</td>
	<td class='dolaczyl'>{$m.datetime_last|kiedy}</td>
</tr>
{/foreach}
</tbody>
</table>

	</div>
</div>


</div>

<br class='clear' />



{include file="_footer.tpl"}
{/if}