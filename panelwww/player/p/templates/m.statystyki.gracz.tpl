{* Smarty *}
{assign var=p value=$RDB->pobierz_wyniki("select p.id,p.nick,p.last_skin,p.respect,p.skill,p.gamep,p.datetime_last,p.datetime_registered,p.level,(p.vip>=NOW()) vip FROM fs_players p WHERE p.id=$pid")}
{assign var=nick value=$p.nick|htmlspecialchars}
{addbc txt="Gracz $nick" url="/stats/player/$pid"}
{if $p.id>0}

{include file="_header.tpl"}

<div class="fctop20">

{assign var=dom value=$RDB->pobierz_wyniki("select id,X,Y from fs_houses where ownerid=$pid and paidTo>=DATE(NOW());")}
{assign var=domid value=$dom.id}

{assign var=ban value=$RDB->pobierz_wyniki("SELECT TIMESTAMPDIFF(SECOND, NOW(), date_end) pozostalo FROM fs_bans WHERE player_banned = $pid AND date_end > NOW() AND date_created < NOW() LIMIT 1")}

{if $dom.id>0 && $dom.X>-4000 && $dom.X<4000 && $dom.Y>-4000 && $dom.Y<4000}
<div class='fr49 playerhouse ui-widget ui-widget-content ui-corner-top ui-corner-bottom'>
<div id='map'></div>
</div>


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
  var mapType = new SanMapType(2, 2, function (zoom, x, y) {ldelim}
        return x == -1 && y == -1 ? "/s/i/tiles/map.outer.png" : "/s/i/tiles/map." + zoom + "." + x + "." + y + ".png";//Where the tiles are located
  {rdelim});
  
	var point=new google.maps.LatLng(((({$dom.Y})*90)/3000), ((parseFloat({$dom.X})*90)/3000));
  
  //Initialize the map
  var sm = new SanMap('map', {ldelim}'Mapa': mapType{rdelim}, 2, point);
  
  	createMapMarker16(point, 32);

</script>
{/if}



<div class='gracz ui-widget ui-widget-content ui-corner-top ui-corner-bottom fl49	'>

{if $p.last_skin>0}
	<img class='avatar' src='/s/i/skinsfs/{$p.last_skin|intval}.png' alt='' width='80' height='180' />
{/if}


{assign var=zdj value="/home/fullserv/public_html/fs.i32.pl/s/i/domy/dom_{$dom.id|intval}.jpg"}
{if $zdj|file_exists}
<a href='/obsluga?{$dom.id|intval}#domki'><img class='dom' src='http://fs.i32.pl/s/i/domy/dom_{$dom.id|intval}.jpg' width='213' height='160' /></a>
{/if}
<h1>{$p.nick|htmlspecialchars}</h1>
{if $p.vip || ($p.level>0 && $p.level<5) || $ban.pozostalo>0}
<p>
{if $p.level==1}<span class='ranga gm'>Moderator</span> 
{elseif $p.level==2}<span class='ranga rekrut'>Administrator rekrut</span> 
{elseif $p.level==3}<span class='ranga admin'>Administrator serwera DM</span> 
{elseif $p.level==4}<span class='ranga rcon'>Administrator RCON</span> {/if}
{if $p.vip}<span class='ranga vip'>V.I.P.</span> {/if}
{if $ban.pozostalo>0}<span class='banned'>Ten gracz jest zbanowany!</span> {/if}
</p>
{/if}

{assign var=g value=$RDB->pobierz_wyniki("select pig.rank,g.id,g.name,g.tag,g.color from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid;")}
{if $g.id>0}
<p class='gang'>
	{if $g.rank=='owner'}Założyciel
	{elseif $g.rank=='viceowner'}Współzałożyciel
	{elseif $g.rank=='leader'}Lider
	{else}Członek{/if}
	<a href='/stats/gang/{$g.id}' style='color: #{$g.color};'>{$g.name}</a>.
</p>
{/if}


<p>Respekt: <b>{$p.respect}</b>{if $p.skill>0}, skill: <b>{$p.skill}</b>, GameP: <b>{$p.gamep}</b>{/if}.
<br />Ostatnio aktywny: <b>{$p.datetime_last|kiedy}</b>.
<br />Gracz od: <b>{$p.datetime_registered|kiedy2}</b>.
</div>




<br class='clear margin' />


<div class="tabs statygracza">
	{assign var=r value=$RDB->pobierz_tabele_wynikow("SELECT rr.raceid,r.descr,r.distance,r.vehicle,MIN(recordtime) besttime,AVG(recordtime) avgtime,count(recordtime) tries FROM fs_races_records rr JOIN fs_races r ON r.id=rr.raceid WHERE rr.playerid=$pid AND r.active=1 GROUP BY rr.raceid;")}
	{assign var=d value=$RDB->pobierz_tabele_wynikow("SELECT rr.raceid,r.descr,r.distance,r.vehicle,MAX(finalscore) best,AVG(finalscore) avg,count(finalscore) tries FROM fs_drift_records rr JOIN fs_drift_tracks r ON r.id=rr.raceid WHERE rr.playerid=$pid AND r.active=1 GROUP BY rr.raceid;")}
	{assign var=pas value=$RDB->pobierz_tabele_wynikow("select a.id,a.descr,sum(pa.kills) score from fs_arena a JOIN fs_players_arenascore pa ON a.id=pa.id_arena WHERE pa.id_player=$pid GROUP BY a.id;")}
	{assign var=a value=$RDB->pobierz_tabele_wynikow("SELECT a.shortname,a.name,pa.score FROM fs_players_achievements pa JOIN fs_achievements a ON a.shortname=pa.shortname WHERE pa.playerid=$pid;")}
	{assign var=polecenia value=$RDB->pobierz_tabele_wynikow("SELECT po.polecona,p1.nick polecona_nick,p1.respect polecona_respect,po.ts FROM fs_polecenia po JOIN fs_players p1 ON p1.id=po.polecona WHERE po.polecil=$pid ORDER BY po.ts DESC;")}


	<ul>
		{if $r|count>0}<li><a href='#wyscigi'>Wyścigi z miesiaca</a></li>{/if}
		{if $d|count>0}<li><a href='#drifting'>Drifting z miesiaca</a></li>{/if}
		{if $pas|count>0}<li><a href='#areny'>Areny DM</a></li>{/if}
		{if $a|count>0}<li><a href='#osiagniecia'>Osiągnięcia</a></li>{/if}
		{if $polecenia|count>0}<li><a href='#polecenia'>Polecenia</a></li>{/if}
		<li><a href='#paski'>Paski</a></li>
	</ul>

	{if $r|count>0}
	<div id='wyscigi' class='wyscigi'>
			<ul class='rr'>
			{foreach from=$r item=rr}
				<li>
					<img src='/engine/fsdomy/s/veh/Vehicle_{$rr.vehicle|intval}.jpg' width="68" height="41" />
					<h3><a href='/stats/race/{$rr.raceid|intval}'>{$rr.descr|htmlspecialchars}</a></h3>
{*					<p>Odległość: <b>{$rr.distance}km</b></p>*}
					<p>Najlepszy czas: <b>{$rr.besttime|string_format:"%.2f"}s</b>{if $rr.tries>1}, czas średni: <b>{$rr.avgtime|string_format:"%.2f"}s</b>{/if}.</p>
				</li>
			{/foreach}

			</ul>
	</div>
	{/if}


	{if $d|count>0}
	<div id='drifting' class='drifting wyscigi'>
			<ul class='rr'>
			{foreach from=$d item=rr}
				<li>
					<img src='/engine/fsdomy/s/veh/Vehicle_{$rr.vehicle|intval}.jpg' width="68" height="41" />
					<h3><a href='/stats/drift/{$rr.raceid|intval}'>{$rr.descr|htmlspecialchars}</a></h3>
{*					<p>Odległość: <b>{$rr.distance}km</b></p>*}
					<p>Najlepszy wynik: <b>{$rr.best}p</b>{if $rr.tries>1}, średni: <b>{round($rr.avg)}p</b>{/if}.</p>
				</li>
			{/foreach}

			</ul>
	</div>
	{/if}


	{if $pas|count>0}
	<div id='areny' class="areny">
		<table class="pas datatable ui-widget-content eui-corner-top">
			<thead>
				<tr>
					<th width='2%' class="nosort">&nbsp;</th>
					<th>Arena</th>
					<th>Fragów</th>
					<th>Pozycja</th></tr>
			</thead>
			<tbody>
			{foreach from=$pas item=as key=kpas}
				<tr>
					<td><a href="/stats/areny#arena-{$as.id|intval}"><img src='/s/i/areny/arenax45-{$as.id|intval}.jpg' width='72' height='45' alt='{$as.descr|htmlspecialchars}' /></a></td>
					<td><a href="/stats/areny#arena-{$as.id|intval}">{$as.descr|htmlspecialchars}</a></td>
					<td align='right'>{$as.score|intval}</td>
					<td align='right'>{assign var=aswynik value=$as.score|intval}
						{assign var=asarena value=$as.id|intval}
						{assign var=pozycja value=$RDB->wynik("select count(id_player)+1 pozycja from fs_players_arenascore where id_arena=$asarena and kills>$aswynik")}
						{if $pozycja<=10}{assign var=arenascore value=1}{/if}
						{$pas[$kpas].pozycja=$pozycja}
						{$pozycja|intval}.
					</td>
				</tr>
			{/foreach}
			</tbody>
		</table>
	</div>
	{/if}

	{if $a|count>0}
	<div id='osiagniecia'>
			{if $arenascore>0}
			<ul class='osiagniecia'>
			{foreach from=$pas item=as}{if $as.pozycja==1}
				<li><img src='/s/i/gold_medal.png' width='48' height='48' /><p>Najlepszy na<br /><b>{$as.descr|htmlspecialchars}</b>.</p></li>
			{else if $as.pozycja==2}
				<li><img src='/s/i/silver_medal.png' width='48' height='48' /><p><b>{$as.descr|htmlspecialchars}</b><br />drugie miejsce.</p></li>
			{else if $as.pozycja==3}
				<li><img src='/s/i/bronze_medal.png' width='48' height='48' /><p><b>{$as.descr|htmlspecialchars}</b><br />trzecie miejsce.</p></li>
			{else if $as.pozycja<=10}
				<li><img src='/s/i/arrow_top.png' width='48' height='48' /><p><b>{$as.descr|htmlspecialchars}</b><br />TOP10.</p></li>
			{/if}{/foreach}
			</ul>
			{/if}
			{foreach from=$a item=o}
				{assign var=rank value=$RDB->pobierz_wyniki("SELECT ar.rank FROM fs_players_achievements pa JOIN fs_achievements a ON pa.shortname=a.shortname JOIN fs_achievements_ranks ar ON ar.shortname=pa.shortname AND ar.score<=pa.score WHERE pa.playerid=$pid AND pa.shortname='{$o.shortname}' ORDER BY ar.score DESC LIMIT 1")}
				{assign var=score value=$RDB->pobierz_wyniki("SELECT ar.score FROM fs_players_achievements pa JOIN fs_achievements a ON pa.shortname=a.shortname JOIN fs_achievements_ranks ar ON ar.shortname=pa.shortname AND ar.score>=pa.score WHERE pa.playerid=$pid AND pa.shortname='{$o.shortname}' ORDER BY ar.score ASC LIMIT 1")}
				<p>{$o.name}: <b>{$rank.rank}</b> 
				
				{if $score.score>0}
					(punkty: <i>{$o.score}</i> z <i>{$score.score}</i> do następnego poziomu)</p>
				{else}
					(punkty: <i>{$o.score}</i>, osiagnieto maksymalny poziom)</p>
				{/if}
			{/foreach}
	</div>
	{/if}

	{if $polecenia|count>0}
	<div id='polecenia'>
		<p>{$nick} polecił/-a FullServer następującym osobom:</p>
		<ul>
		{foreach from=$polecenia item=pl}
		<li><i>{$pl.ts|kiedy}</i> <a href='/stats/player/{$pl.polecona|intval}'>{$pl.polecona_nick|htmlspecialchars}</a> ({$pl.polecona_respect} respektu).
		</li>
		{/foreach}
		</ul>
		<p><a href='/polec'>Poleć serwer znajomym</a>, za każdą poleconą osobę, która uzyska 2000 respektu otrzymasz 1000 respektu oraz 10 dni konta VIP!</p>
	</div>
	{/if}

	<div id='paski'>

		<h3>Avatar na forum</h3>
		<p>Skopiuj bezpośredni adres URL i wstaw w ustawieniach swojego profilu na forum aby mieć zawsze aktualny avatar.</p>
		<div>
			<p><img src='http://fs.i32.pl/player/2-{$pid}.png' width='140' height='160' alt='{$p.nick|htmlspecialchars}' /></p>
			<p>URL bezpośredni: <input type='text' class='copy' value="http://fs.i32.pl/player/2-{$pid}.png" /></p>
		</div>


		<h3>Paski/userbary</h3>
		<p>Aby wstawić pasek/userbar do swojej stopki, skopiuj kod BBCode i wstaw do swojego podpisu w swoim profilu na forum.</p>
		<div>
			<p><img src='http://fs.i32.pl/player/{$pid}.png' width='450' height='33' alt='{$p.nick|htmlspecialchars}' /></p>
			<p>BBCode: <input type='text' class='copy' value="[url=http://fs.i32.pl/stats/player/{$pid}][img]http://fs.i32.pl/player/{$pid}.png[/img][/url]" /></p>
			<p>HTML: <input type='text' class='copy' value="&lt;img src='http://fs.i32.pl/player/{$pid}.png' width='450' height='33' alt='{$p.nick|htmlspecialchars}' /&gt;" /></p>
			<p>URL bezpośredni: <input type='text' class='copy' value="http://fs.i32.pl/player/{$pid}.png" /></p>
		</div>

		{for $pp=1 to 5}
		<div>
			<p><img src='http://fs.i32.pl/player/3-{$pid}-{$pp}.png' width='350' height='20' alt='{$p.nick|htmlspecialchars}' /></p>
			<p>BBCode: <input type='text' class='copy' value="[url=http://fs.i32.pl/stats/player/{$pid}][img]http://fs.i32.pl/player/3-{$pid}-{$pp}.png[/img][/url]" /></p>
			<p>HTML: <input type='text' class='copy' value="&lt;img src='http://fs.i32.pl/player/3-{$pid}-{$pp}.png' width='350' height='20' alt='{$p.nick|htmlspecialchars}' /&gt;" /></p>
		</div>
		{/for}

	</div>

</div>


</div>
<br class='clear' />


{include file="_footer.tpl"}
{/if}
