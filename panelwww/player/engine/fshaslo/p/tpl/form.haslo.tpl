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


<form class='validate' method='POST' action='/engine/fshaslo/index.php'><fieldset id='vipbuy'><legend>Przypomnij hasło do konta</legend>

<div style="float: right; width: 20%; background: white; padding: 0.5em; border-radius: 1em;">
Twoje nowe hasło jest wysyłane na adres email podany przy rejestracji. Jeżeli nadal masz problem, napisz <a href="http://fullserver.eu/forum/18-pro%C5%9Bby-o-przywr%C3%B3cenie-respektu-zresetowanie-has%C5%82a/">w tym dziale</a>
</div>

<p class='nick'><label>Twój nick w grze:</label>
    <input type='text' name='nick' class='required' maxlength='20' minlength='3' /></p>


<p class='nick'><label>Adres e-mail:</label>
    <input type='text' name='email' class='required' />
<button type='submit'>Dalej &raquo;</button></p>

<hr class='clear' />

</fieldset>
</form>
