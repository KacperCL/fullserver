{* Smarty *}

<fieldset id='ok'><legend>Zgłoszenie przyjęte</legend>
<p>Twoje zgłoszenie zostało zarejestrowane i pomyslnie przetworzone!</p>
<p>Wiadomość email z nowym hasłem powinna dojść do Twojej skrzynki pocztowej w ciągu <strong>24h</strong> (zazwyczaj <strong>natychmiast</strong>). W razie problemów sprawdź folder <strong>SPAM</strong>.</p>

<p>Na wszelki wypadek zapisz/wydrukuj tą stronę.</p>


<dl>
<dt>Nick w grze:</dt>
<dd>{$f_nick}</dd>

<dt>Adres e-mail podany przy rejestracji:</dt>
<dd>{$f_email}</dd>

<dt>Data:</dt>
<dd>{$smarty.now|date_format:"%Y.%m.%d %H:%M"}</dd>

</dl>
<br />
<p class='clear' /></p>
<br />
<button onclick='document.location="/fshaslo/";'>&laquo; Wróć</button>
<button onclick='window.print();'>Drukuj</button>

<p class='clear' /></p>
</fieldset>
