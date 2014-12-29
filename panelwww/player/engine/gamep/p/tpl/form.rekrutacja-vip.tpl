{* Smarty *}

{if $bledy}
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


<form class='validate' method='POST' action='/engine/gamep/index.php'><fieldset id='vipbuy'><legend>Doładuj GamePoints</legend>

<div style="float: right; width: 20%; background: white; padding: 0.5em; border-radius: 1em;">
Twoje saldo GamePoints jest wirtualna waluta pozwalajaca na zakup uslug na serwerze. Aby wykorzystac zakupione punkty wejdz na serwer i wpisz komende <b>/GP</b> a nastepnie wybierz odpowiednia opcje (np. zakup VIP)
</div>

<p class='nick'><label>Twój nick w grze:</label>
    <input type='text' name='nick' class='required' maxlength='20' minlength='3' /></p>


<p class='nick'><label>Adres e-mail:</label>
    <input type='text' name='email' class='required' />
</p>

    
<p><label>Ilość dni:</label>
    <select autocomplete='off' name='gamep'>
{foreach from=$pakiety item=p key=idx}
    <option {if $idx==690}selected="true"{/if} value='{$idx}' numer='{$p.numer}' kod='{$p.kod}' koszt='{$p.koszt}'>{$p.gamep} punktów - {$p.koszt}zł</option>
{/foreach}
	    </select></p>

<p class='cb'><label><input type='checkbox' name='regulamin' class='required' /> akceptuję <a href='http://fs.i32.pl/engine/gamep/s/regulamin.doc' target='_blank' rel='nofollow'>Regulamin Usług i serwisów SMS</a></label></p>

<hr class='clear' />

<div id='smsinfo' class='cols2'>
    <div>
    	
        <p>Aby dokonać opłaty, wyślij:</p>
        <a href='smsto:-:---------'><img class='qrcode' src='http://chart.apis.google.com/chart?cht=qr&chs=120x120&chl=smsto%3A0%3A-' width='120' height='120' alt='QR kod. Zeskanuj aby wysłac SMS.' /></a>
        <h2>-</h2>
        <p>Pod numer:</p>
        <h3>-</h3>
	<p>Koszt:</p>
	<h4>-</h4>
	<br />
	<p>Otrzymany kod:</p>
	<p><input type='text' name='kod' class='required' maxlength='8' minlength='8' size='8' /> <button type='submit' >Dalej &raquo;</button></p>
    </div>
    <div>
	<p>Informacje o płatności SMS Premium:</p>
	<ul>
		<li>Płatność obsługiwana jest przez JustPay</li>
		<li>Usługa dostępna jest we wszystkich polskich sieciach komórkowych</li>
		<li>Podane ceny, są cenami brutto</li>
	</ul>
	<p>Zobacz również:</p>
	<ul>
		<li><a rel='nofollow' href='http://fs.i32.pl/engine/gamep/s/regulamin.doc'>Regulamin Usług i serwisów SMS</a></li>
		<li><a rel='nofollow' href='http://fullserver.eu/index.php?app=members&module=messaging&section=send&do=form&fromMemberID=11934'>Problemy z realizacją płatności</a></li>
		<li><a href='mailto:eider@fullserver.eu'>Kontakt z właścicielem</a></li>
	</ul>
    </div>
    
</div>


</fieldset>
</form>

<script>{literal}
$(function() {
    var select = $("select[name=gamep]");
    var slider = $('<p id="slider"></p>').insertBefore(select).slider({
	min: 1,
        max: 8,
	range: "min",
	value: 4,
	slide: function(event, ui) {
		var wybrana=$("select[name=gamep]>option:eq("+Number(ui.value-1)+")").val();
		$("select[name=gamep]").val(wybrana);
		pokaz_pakiet();
	}
});

$("select[name=gamep]").bind('blur change',slider_setselection);

function slider_setselection() {


		slider.slider("value", $("select[name=gamep]>option:selected").index()+1);
		pokaz_pakiet( $("select[name=gamep]").val());

};

});
																		    
{/literal}</script>
