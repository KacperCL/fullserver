{* Smarty *}

{include file="_header.tpl"}
<div class="fctop20">
<div id='polec'>
<h1>Poleć FullServer - otrzymaj nagrodę</h1>
{assign var=nagrodzonych value=$RDB->wynik("select count(*)*2 nagrodzonych from fs_polecenia where uniewazniony=0 and rewarded=1")}
<h2>Rozdaliśmy już {$nagrodzonych*1000} respektu i {$nagrodzonych*10} dni <span style='color: yellow;'>VIPa</span></h3>

<div class='fl49'>
<ol class='poleckorzysci'>
	<li><b>Poleć FullServer swojemu znajomemu!</b>
		<span>Zainstaluj SA:MP koledze, koleżance, pomóż mu założyć konto i zapoznać się z FullServerem.</span></li>
	<li><b>Wypełnijcie formularz na tej stronie</b><span>tak abyśmy wiedzieli, że to Ty poleciłeś/-aś tego gracza.</span></li>
	<li><b>Za polecenie, otrzymacie 1000 respektu i 10-dniowe konto <span style='color: yellow;'>VIP</span></b><span>w momencie gdy osoba polecona osiągnie 2000 respektu! Nagrodę dostaje zarówno osoba polecająca jak i polecona!</span></li>
{*	<li><b>Masz szansę otrzymać rangę moderatora</b><span>zostanie ona przyznana osobie, która do końca listopada poleci nasz serwer największej ilości osób.</span></li>*}
	<li><b>Korzyści się kumulują!</b><span>Poleć FullServer 10 osobom, otrzymasz 10k respektu i 100 dni VIPa! Nie ma ograniczeń w ilości.</span></li>
</ol>

<p>Uwagi: naliczanie punktów respektu i doładowań VIP odbywa się co godzinę w chwili nieaktywności obydwu stron. Pamiętaj, że niedopuszczalne jest spamowanie na innych serwerach tudzież inne
agresywne formy reklamy. Osoby polecone będą weryfikowane pod względem autentyczności, nie opłaca się więc oszukiwać. Formularz może wypełnić tylko osoba, ma więcej niż 25, a mniej niż 1000 respektu.
Pytania można zadawać <a href='http://fullserver.eu/index.php?topic=22859.msg183001#msg183001'>na forum</a>.
</p>
</div>

<div class='fr49'>


<form method='POST'><fieldset class='polecenie'><legend>Zgłoszenie polecenia osoby</legend>
{if $polecenia_done}
	<div>Gratulacje! Informacje o poleceniu zostały zapisane :-). Dobij teraz do 2k respektu, aby otrzymać dodatkowy respekt i 10 dni konta VIP!<br />
	Pamiętaj, że również możesz komuś polecić FullServer i zyskać dodatkowy respekt i dodatkowe doładowanie!</div>
{else}
	<div>Wypełnia osoba polecona!</div>
	<p>
		<label>Twój nick na serwerze:</label>
		<input type='text' name='polecona_nick' id='polecona_nick' value='{$smarty.post['polecona_nick']|htmlspecialchars}' />
	</p>
	<p>
		<label>Nick osoby, która poleciła Ci FullServer</label>
		<input type='text' name='polecil_nick' id='polecil_nick' value='{$smarty.post['polecil_nick']|htmlspecialchars}'/>
	</p>
	
	{if $polec_blad}
	<p class='blad'>
		{$polec_blad}
	</p>
	{/if}

	<p align='center'>
		<button type='submit'>Wyślij zgłoszenie</button>
	</p>
{/if}
</fieldset></form>

<h3>Ostatnie polecenia:</h3>

<ul>
{foreach from=$polecenia_log item=pl}
<li>
	{if $pl.uniewazniony}<strike>{/if}
	<i>{$pl.ts|kiedy}</i> <a href='/stats/player/{$pl.polecil|intval}'>{$pl.polecil_nick|htmlspecialchars}</a> polecił <a href='/stats/player/{$pl.polecona|intval}'>{$pl.polecona_nick|htmlspecialchars}</a> ({$pl.polecona_respect} respektu).
	{if $pl.uniewazniony}
		</strike>
		{if $pl.uniewazniony_powod}<acronym title="Powód unieważnienia: {$pl.uniewazniony_powod|htmlspecialchars}">?</acronym>{/if}
	{/if}
</li>
{/foreach}
</ul>

</div>

<br class='clear' />

<div class='fl49'>
<h3>Wkrótce nagrodzeni zostaną:</h3>
{assign var=wkrotce value=$RDB->pobierz_tabele_wynikow("SELECT po.polecil,p2.nick polecil_nick,po.polecona,p1.nick polecona_nick,p1.respect polecona_respect,po.ts                              FROM fs_polecenia po JOIN fs_players p1 ON p1.id=po.polecona JOIN fs_players p2 ON p2.id=po.polecil WHERE uniewazniony=0 AND rewarded=0 ORDER BY polecona_respect DESC LIMIT 5;")}
<ul>
{foreach from=$wkrotce item=w}
<li><a href='/stats/player/{$w.polecona|intval}'>{$w.polecona_nick|htmlspecialchars}</a> i <a href='/stats/player/{$w.polecil|intval}'>{$w.polecil_nick|htmlspecialchars}</a> - {if $w.polecona_respect<2000}jeszcze {2000-$w.polecona_respect|intval}{else}{$w.polecona_respect|intval}{/if}p. respektu</li>
{/foreach}
</ul>

<h3>Najwięcej poleceń wykonali:</h3>
{assign var=thebest value=$RDB->pobierz_tabele_wynikow("select fp.polecil,fpl.nick,count(fp.polecona) polecen from fs_polecenia fp JOIN fs_players fpl ON fp.polecil=fpl.id where fp.uniewazniony=0 and fp.rewarded=1 GROUP BY fp.polecil ORDER BY polecen DESC LIMIT 10;")}
<ul>
{foreach from=$thebest item=t}{if $t.polecen>1}
<li><a href='/stats/player/{$t.polecil|intval}'>{$t.nick|htmlspecialchars}</a> - {$t.polecen|intval|dli:"osoba":"osoby":"osób"}</li>
{/if}{/foreach}
</ul>

</div>

<div class='fr49'>
<h3>Ostatnio nagrodzeni zostali:</h3>
{assign var=ostatnio value=$RDB->pobierz_tabele_wynikow("SELECT reward_date,po.polecil,p2.nick polecil_nick,po.polecona,p1.nick polecona_nick,p1.respect polecona_respect,po.ts                              FROM fs_polecenia po JOIN fs_players p1 ON p1.id=po.polecona JOIN fs_players p2 ON p2.id=po.polecil WHERE uniewazniony=0 AND rewarded=1 ORDER BY reward_date DESC LIMIT 10;")}
<ul>
{foreach from=$ostatnio item=w}
<li>{$w.reward_date|kiedy} <a href='/stats/player/{$w.polecona|intval}'>{$w.polecona_nick|htmlspecialchars}</a> i <a href='/stats/player/{$w.polecil|intval}'>{$w.polecil_nick|htmlspecialchars}</a></li>
{/foreach}
</ul>
</div>




</div>
</div>



{include file="_footer.tpl"}