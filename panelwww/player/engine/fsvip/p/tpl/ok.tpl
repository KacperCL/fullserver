{* Smarty *}

<fieldset id='ok'><legend>Zgłoszenie przyjęte</legend>
<p>Twoje zgłoszenie zostało zarejestrowane i pomyslnie przetworzone!</p>
<p><b>Twoje konto VIP zostalo automatycznie przedluzone o {$f_iloscdni} dni!</b></p>

<p>Na wszelki wypadek zachowaj otrzymany kod lub zapisz/wydrukuj tą stronę.</p>


<dl>
<dt>Nick w grze:</dt>
<dd>{$f_nick}</dd>

<dt>Adres e-mail:</dt>
<dd>{$f_email}</dd>

<dt>Opłacona ilość dni:</dt>
<dd>{$f_iloscdni}</dd>

<dt>Forma płatności:</dt>
<dd>SMS <b>{$pakiety.$f_iloscdni.kod}</b> na numer <b>{$pakiety.$f_iloscdni.numer}</b></dd>

<dt>Kod aktywacyjny:</dt>
<dd><b>{$f_kod}</b></dd>

<dt>Data:</dt>
<dd>{$smarty.now|date_format:"%Y.%m.%d %H:%M"}</dd>

</dl>
<br />
<p class='clear' /></p>
<br />
<button onclick='document.location="/fsvip/";'>&laquo; Wróć</button>
<button onclick='window.print();'>Drukuj</button>

<p class='clear' /></p>
</fieldset>
