{include file="html.top.tpl"}

<div align="center" style="padding-left: 40px; padding-right: 40px">

{if $info}<div class="info">{$info}</div>{else}<div class="info">Wybierz plik do pobrania i poczekaj az przegladarka zglosi okno pobierania pliku</div>{/if}
{if $success}<div class="success">{$success}</div>{/if}
{if $warn}<div class="warning">{$warn}</div>{/if}
{if $blad}<div class="error">{$blad}</div>{/if}
<br/><br/>
Wybierz plik:
<form method="POST" action="">
<select name="plik">
  {if $LVL==4}<option value="server_log.txt">server_log.txt</option>{/if}
  <option value="scriptfiles/actions_log.txt" selected="selected">scriptfiles/actions_log.txt</option>
</select> 
<br/><br/>
<input type="submit" value="Pobierz" />
</form>

</div>

{include file="html.bottom.tpl"}