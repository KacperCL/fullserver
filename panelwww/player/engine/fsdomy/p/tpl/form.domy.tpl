{* Smarty *}

<script>
    var house_x='{$dom->X}';
    var house_y='{$dom->Y}';
    var house_id={$dom->id};
    {if $dom->wolny}
    var house_wolny=1;
    {else}
    var house_wolny=0;
    {/if}
    
</script>

<p id='nav'>
	{if $dom->prevFID}<a rel="prev" href='{$dom->prevFID|intval}'>poprzedni wolny</a>{/if}
	{if $dom->prevID}<a rel="prev" href='{$dom->prevID|intval}'>&laquo;</a>{/if}
	<a rel="index" href='/engine/fsdomy'>lista</a>
	{if $dom->nextID}<a rel="next" href='{$dom->nextID|intval}'>&raquo;</a>{/if}
	{if $dom->nextFID}<a rel="next" href='{$dom->nextFID|intval}'>następny wolny</a>{/if}
</p>



{* {if $bledy}
<fieldset id='bledy'><legend>Błąd!</legend>
<p>Podczas przetwarzania formularza wykryto {if $bledy|@count==1}błąd:{else}następujące błędy:{/if}</p>
<ul>
{foreach from=$bledy item=b}
    <li>{$b}</li>
{/foreach}
</ul>
<p class='clear' /></p>
</fieldset>
{/if}
*}

{* <form class='validate' method='POST' action='/engine/fsdomy/index.php'> *}
<fieldset id='homebuy'><legend><h1>{$dom->descr|htmlspecialchars}</h1></legend>

<div id="map"></div>

{if $dom->zdj}
	<img src="http://fs.i32.pl/s/i/domy/320_dom_{$dom->id}.jpg" width="320" height="240" id='zdjecie' />
{/if}

<dl>
    <dt>Wlasciciel:</dt>
    <dd>{if $dom->wolny}brak{else}<a href='http://fs.i32.pl/stats/player/{$dom->ownerid|intval}'>{$dom->owner_nick|htmlspecialchars|default:"brak"}</a>{/if}
</dd>
    
    <dt>Cena:</dt>
    {* <dd><a href='#nizej' class='zoltybtn {if $dom->ownerid==0 || $dom->dni<0}t1{else}t2{/if}'>{if $dom->ownerid==0 || $dom->dni<0}Oplac SMSEM{else}Oplac SMSEM{/if}</a> *}
	{if $dom->koszt>0}
	{$dom->koszt} respektu za dobę,<br />
    
	{$dom->koszt*30.5|string_format:"%01d"} respektu za miesiąc,<br />
	<b>LUB</b> {else}<br />{/if}<i><b>690 GamePoints</b> za miesiąc</i>
	
	</dd>

	<dt>Lokalizacja:</dt>
	<dd>{$dom->zone_name}</dd>
	
	{if !$dom->wolny}
		<dt>Opłacony do:</dt>
		<dd>{$dom->paidTo}</dd>
	{/if}

    <dt>Uwagi:</dt>
    <dd>{if !$dom->wolny}
	{if $dom->vehicles_allowed==0}- brak miejsc parkingowych
	{elseif $dom->vehicle_model>0}- w cenie, zaparkowany pod domem <b>{$dom->vehicle_name}</b>.<br />
		<img src='s/veh/Vehicle_{$dom->vehicle_model}.jpg' alt='' /><br />
		(możliwosc zmiany na dowolny inny pojazd)
	{else}- mozliwosc zapisania pojazdu{/if}
	{else}
	    {if $dom->vehicles_allowed==0}- brak miejsc parkingowych
	    {elseif $dom->vehicle_model>0}- pod domem jest obecnie zaparkowany <b>{$dom->vehicle_name}</b>.<br />
		<img src='s/veh/Vehicle_{$dom->vehicle_model}.jpg' alt='' /><br />
	    {else}- jedno wolne miejsce parkingowe{/if}
	{/if}
	{if $dom->restrict_gang>0}
		<br/>- mozliwy do wynajecia tylko dla czlonkow <b>{$dom->gang_name}</b>.
	{/if}
	</dd>
    
</dl>

{* <h2>Opłać dom za pomocą GamePoints</h2>
<div class='tabs'>
	<ul>
		<li><a href="#sms-pl">Polska</a></li>
		<li><a href="#sms-uk">Wielka Brytania</a></li>
	</ul>
	<div id='sms-pl'>
<div id="homebuy">

{if $dom->restrict_gang>0 && $dom->wolny}
	<p>Zakup tego domu mozliwy jest tylko i wylacznie w grze. Aby dokonac zakupu poprzez wyslanie SMSa, wykup go w grze na 1 dzien a nastepnie wroc na ta strone.</p>
{else}

{if $dom->wolny}<p class='fw'>Możesz opłacić domek na <b>37 dni</b> wysyłając jednego smsa.{if $dom->koszt>0} Zaoszczędzisz w ten sposób <b>{$dom->koszt*37.5|intval}</b> punktów respektu!{/if} Aby to zrobić - wypełnij pola poniżej.</p>{/if}
{if !$dom->wolny}<p class='fw'>Wypełniając poniższy formularz i wysyłając smsa, opłacisz ten dom na kolejne <b>37 dni</b> dla gracza <b>{$dom->owner_nick}</b>. Jeśli zamiast tego chcesz
opłacić dla siebie inny dom, wróc do <a href='/engine/fsdomy/'>listy domów</a>.</p>{/if}
<a name='nizej'></a>
{if $dom->ownerid==0 || $dom->dni<0}
<p class='nick'><label>Twój nick w grze:</label>
    <input type='text' name='nick' class='required' maxlength='20' minlength='3' value='{if $smarty.request.nick}{$smarty.request.nick}{else}{$prawdopodobny_nick}{/if}' /></p>
{/if}

<input type='hidden' name='domid' value='{$dom->id}' />

<p class='nick'><label>Adres e-mail:</label>
    <input type='text' name='email' class='required email' />
</p>
<p class='cb'><label><input type='checkbox' name='regulamin' class='required' /> akceptuję<br /><a href='http://fs.i32.pl/engine/fsdomy/s/regulamin.doc' target='_blank' rel='nofollow'>Regulamin Usług i serwisów SMS</a>.</label></p>

<div id='smsinfo'>
    <div class='cols3'>
    	<div>
	    <a href='smsto:7636:KOD.FSDOM'><img class='qrcode' src='http://chart.apis.google.com/chart?cht=qr&chs=67x67&chl=smsto%3A7636%3AKOD.FSDOM' width='67' height='67' alt='QR kod. Zeskanuj aby wysłac SMS.' /></a>
    	    <p>Wyślij SMS o treści:</p>
            <h2>KOD.FSDOM</h2>
	</div>
	<div>
    	    <p>Pod numer:</p>
    	    <h3>7636</h3>
	</div>
	
	<div>
	    <p>Koszt:</p>
	    <h3>7.38zł</h3>
		<h4>6.00zł netto</h4>
	</div>
        </div>
</div>


<p><label>Otrzymany kod:</label>
<input type='text' name='kod' class='required' maxlength='6' minlength='6' size='6' />
<button id='submitbtn' type='submit'>Opłać &raquo;</button></p>


<div class='cols2' id='bottominfo'>
 <div>
    	<p>Informacje o płatności SMS Premium:</p>
	<ul>
		<li>Płatność obsługiwana jest przez JustPay</li>
		<li>Usługa dostępna jest w sieciach: Orange, Era, Plus, Play, Sferia, Cyfrowy Polsat oraz wszystkich sieci wirtualnych MVNO (np. GaduAir, mBank mobile, Aster).</li>
	</ul>
 </div>
 <div>
	<p>Zobacz również:</p>
	<ul>
		<li><a rel='nofollow' href='http://fs.i32.pl/engine/fsdomy/s/regulamin.doc'>Regulamin usługi</a></li>
		<li><a href='http://fullserver.eu/index.php?app=members&module=messaging&section=send&do=form&fromMemberID=11934'>Problemy z realizacją płatności</a></li>
		<li><a href='mailto:eider@fullserver.eu'>Kontakt e-mail z właścicielem</a></li>
	</ul>
 </div>
</div>


{/if}
</div>
</div>
<div id='sms-uk'>

<p>Na razie nie ma możliwosci zakupu domku poza terenem Polski.</p>
</div>

</div> *}

</fieldset>
</form>