{* Smarty *}
{include file="_header.tpl"}
<div class='tabs'>
	<ul>
		<li><a href='#t1-killers'>Najlepsi zabójcy</a></li>
		<li><a href='#t1-skill'>Największy skill</a></li>
		<li><a href='#t1-respect'>Najwięcej respektu</a></li>
		<li><a href='#t1-bonusy'>Poszukiwacze bonusow</a></li>
		<li><a href='#t1-derby'>Mistrzowie /DERBY</a></li>
		<li><a href='#t1-race'>Mistrzowie /RACE</a></li>
		<li><a href='#t1-piraci'>Najwięksi piraci</a></li>
	</ul>
	<div id='t1-killers'>
		{foreach name=l from=$RDB->pobierz_tabele_wynikow("select p.id,p.nick,pa.score from fs_players_achievements pa join fs_players p on p.id=pa.playerid where pa.shortname='KILLS' order by pa.score DESC limit 60;") item=l}
		{if $smarty.foreach.l.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.l.index+1}>{/if}
		<li><a href='/stats/player/{$l.id|intval}'>{$l.nick|htmlspecialchars}</a><b>{$l.score}</b></li>
		{/foreach}
		<br class='clear' />
	</div>

	<div id='t1-skill'>
		{foreach name=l from=$RDB->pobierz_tabele_wynikow("select id,nick,skill score from fs_players order by skill desc limit 60;") item=l}
			{if $smarty.foreach.l.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.l.index+1}>{/if}
			<li><a href='/stats/player/{$l.id|intval}'>{$l.nick|htmlspecialchars}</a><b>{$l.score}</b></li>
		{/foreach}
	</div>

	<div id='t1-respect'>
		{foreach name=l from=$RDB->pobierz_tabele_wynikow("select id,nick,respect score from fs_players order by respect desc limit 60;") item=l}
			{if $smarty.foreach.l.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.l.index+1}>{/if}
			<li><a href='/stats/player/{$l.id|intval}'>{$l.nick|htmlspecialchars}</a><b>{$l.score}</b></li>
		{/foreach}
	</div>

	<div id='t1-bonusy'>
		{foreach name=l from=$RDB->pobierz_tabele_wynikow("select p.id,p.nick,pa.score from fs_players_achievements pa join fs_players p on p.id=pa.playerid where pa.shortname='WALIZKI' order by pa.score DESC limit 60;") item=l}
			{if $smarty.foreach.l.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.l.index+1}>{/if}
			<li><a href='/stats/player/{$l.id|intval}'>{$l.nick|htmlspecialchars}</a><b>{$l.score}</b></li>
		{/foreach}
	</div>
	<div id='t1-derby'>
		{foreach name=l from=$RDB->pobierz_tabele_wynikow("select p.id,p.nick,pa.score from fs_players_achievements pa join fs_players p on p.id=pa.playerid where pa.shortname='DERBYWIN' order by pa.score DESC limit 60;") item=l}
			{if $smarty.foreach.l.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.l.index+1}>{/if}
			<li><a href='/stats/player/{$l.id|intval}'>{$l.nick|htmlspecialchars}</a><b>{$l.score}</b></li>
		{/foreach}
	</div>
	
	<div id='t1-race'>
		{foreach name=l from=$RDB->pobierz_tabele_wynikow("select p.id,p.nick,pa.score,pa2.score score2 from fs_players_achievements pa join fs_players p on p.id=pa.playerid join fs_players_achievements pa2 on p.id=pa2.playerid where pa.shortname='RACEFCNT' AND pa2.shortname='RACEDIST' order by pa.score DESC limit 60") item=l}
			{if $smarty.foreach.l.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.l.index+1}>{/if}
			<li><a href='/stats/player/{$l.id|intval}'>{$l.nick|htmlspecialchars}</a><b>{$l.score}, {$l.score2}km</b></li>
		{/foreach}
	</div>


	<div id='t1-piraci'>
		{foreach name=l from=$RDB->pobierz_tabele_wynikow("select p.id,p.nick,pa.score from fs_players_achievements pa join fs_players p on p.id=pa.playerid where pa.shortname='FOTOCASH' order by pa.score DESC limit 60;") item=l}
			{if $smarty.foreach.l.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.l.index+1}>{/if}
			<li><a href='/stats/player/{$l.id|intval}'>{$l.nick|htmlspecialchars}</a><b>{$l.score}$</b></li>
		{/foreach}
	</div>




</div>


{include file="_footer.tpl"}