{* Smarty *}
{include file="_header.tpl"}

{assign var=areny value=$RDB->pobierz_tabele_wynikow("SELECT id,descr FROM fs_arena")}
<div class='tabs'>
	<ul>
		{foreach from=$areny item=arena}
			<li><a href='#arena-{$arena.id|intval}'>{$arena.descr|htmlspecialchars}</a></li>
		{/foreach}
	</ul>
	{foreach from=$areny item=arena}
		{assign var=arenaid value=$arena.id|intval}
		<div id='arena-{$arenaid}'>

{assign var=as value=$RDB->pobierz_tabele_wynikow("select pa.id_player,p.nick,pa.kills from fs_players_arenascore pa JOIN fs_players p ON p.id=pa.id_player where pa.id_arena=$arenaid order by pa.kills desc limit 60;")}

		{foreach name=fa from=$as item=a}
			{if $smarty.foreach.fa.index%15==0}</ol><ol class='zestawienie' start={$smarty.foreach.fa.index+1}>{/if}
			<li><a href='/stats/player/{$a.id_player|intval}'>{$a.nick|htmlspecialchars}</a><b>{$a.kills|intval}</b></li>
		{/foreach}
		</ol>
		<hr class="poziom">
		<div style="clear:both">
{assign var=asg value=$RDB->pobierz_tabele_wynikow("select pig.id_gang,g.tag,g.color,g.name,sum(pa.kills) kills from fs_players_arenascore pa JOIN fs_players_in_gangs pig ON pa.id_player=pig.id_player JOIN fs_gangs g ON g.id=pig.id_gang where pa.id_arena=$arenaid GROUP BY pig.id_gang order by sum(pa.kills) desc limit 100;")}
		{foreach name=fa from=$asg item=a}
			{if $smarty.foreach.fa.index%8==0}</ol><ol class='zestawienie' start={$smarty.foreach.fa.index+1}>{/if}
			<li><a style='color: #{$a.color}; text-shadow: 0 0 1px #000000' href='/stats/gang/{$a.id_gang|intval}'>{$a.tag|htmlspecialchars}</a><b>{$a.kills|intval}</b></li>
		{/foreach}
		</div>
</div>
	{/foreach}
</div>




{include file="_footer.tpl"}