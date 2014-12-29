{include file="html.top.tpl"}

<div align="center" style="padding-left: 40px; padding-right: 40px">

{if $info}<div class="info">{$info}</div>{else}<div class="info">Wypełnij uważnie wszystkie pola. Nie przesadź z datą zakończenia bana. System autmatycznie wykryje czy podałeś IP czy nick gracza.</div>{/if}
{if $success}<div class="success">{$success}</div>{/if}
{if $warn}<div class="warning">{$warn}</div>{/if}
{if $blad}<div class="error">{$blad}</div>{/if}
<br/><br/>
<form method="POST" action="">
Nick gracza/IP do zbanowania:
<br/>
<input type='text' name='nick' id='nick' />
<br/><br/>
Data wygaśnięcia:<br/>
<input type="text" name="data" id="datepicker">
<br/><br/>
Powód bana:<br/>
<input type="text" id="powod" name="powod">
<br/><br/>
<input type="submit" value="Dodaj bana" />
</form>

</div>

{include file="html.bottom.tpl"}