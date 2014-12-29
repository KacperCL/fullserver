{* Smarty *}


<fieldset id='ok'><legend>Zgłoszenie przyjęte</legend>
<p>Twoje zgłoszenie zostało zarejestrowane i pomyslnie przetworzone!</p>
<p><b>Wskazany dom został oplacony! Gratulacje.</b></p>

<p>Na wszelki wypadek zachowaj otrzymany kod lub zapisz/wydrukuj tą stronę.</p>


<dl>
{if $f_nick}
<dt>Nick:</dt>
<dd>{$f_nick}</dd>
{/if}

<dt>Dom:</dt>
<dd>{$dom->descr} (ID:{$dom->id})</dd>

<dt>Adres e-mail:</dt>
<dd>{$f_email}</dd>

<dt>Opłacona ilość dni:</dt>
<dd>30</dd>

<dt>Forma płatności:</dt>
<dd>SMS <b>KOD.FSDOM</b> na numer <b>7636</b></dd>

<dt>Kod aktywacyjny:</dt>
<dd><b>{$f_kod}</b></dd>

<dt>Data:</dt>
<dd>{$smarty.now|date_format:"%Y.%m.%d %H:%M"}</dd>

</dl>
<br />
<p class='clear' /></p>
<br />
<button onclick='document.location="/fsdomy/";'>&laquo; Wróć</button>
<button onclick='window.print();'>Drukuj</button>

<p class='clear' /></p>
</fieldset>
