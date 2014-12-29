{* Smarty *}

{if $bledy}
<fieldset id='bledy'><legend>Błąd!</legend>
<p>Podczas przetwarzania formularza wykryto {if $bledy|@count==1}błąd:{else}następujące błędy:{/if}</p>
<ul>
{foreach from=$bledy item=b}
    <li>{$b}</li>
{/foreach}
</ul>

<br />
<button onclick='{literal}$("#bledy").fadeOut(500,function(){$(this).remove();});$("#vipinfo,#vipbuy").fadeIn(250);{/literal}'>&laquo; wróć</button>
<p class='clear' /></p>
</fieldset>
{/if}
