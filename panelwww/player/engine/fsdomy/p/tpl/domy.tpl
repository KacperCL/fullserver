{* Smarty *}
<script>
    var house_x=0;
    var house_y=0;
    var house_id=-1;
    var house_wolny=0;
</script>

<fieldset id='homelist'>
    <legend>Lista domów</legend>


    <div id='map'></div>
	<div id='hpreview'></div>


        <p style='width: 40%;'>Wybierz swój dom aby móc przedłużyć jego ważność za pomocą SMSa. Wybierz jeden z wolnych domów, aby móc go wykupić.</p>

<form>
<div class="radioset" id='zakresdomow'>
		<input type="radio" id="domyall" name="radio" onclick='zd_zmien()' onchange='zd_zmien()' /><label for="domyall">Wszystkie domy</label>
		<input type="radio" id="domyz" name="radio" onclick='zd_zmien()' onchange='zd_zmien()' ><label for="domyz">Zajęte</label>
		<input type="radio" id="domyw" name="radio" onclick='zd_zmien()' onchange='zd_zmien()'  checked="checked" /><label for="domyw">Wolne</label>
<!--		<input type="radio" id="domyg" name="radio" onclick='zd_zmien()' onchange='zd_zmien()'/><label for="domyg">Gangowe</label> -->
</div>
</form>

<table class='domy'>
<thead>
<tr>
    <th colspan='2'>Lokalizacja</th>
    <th>Właściciel</th>
    <th>Koszt</th>
</tr>
</thead>
<tbody>
{foreach from=$domy->listaDomow() item=d}
<tr hid='{$d.id}' X='{$d.X}' Y='{$d.Y}' class='{if $d.ownerid==0 || $d.dni<0}{if $d.restrict_gang>0}gwolny{else}wolny{/if}{else}{if $d.restrict_gang>0}gzajety{else}zajety{/if}{/if}' onclick='document.location="/engine/fsdomy/{$d.id}" '>
<td><a href='/engine/fsdomy/{$d.id}' title='Dom: {$d.descr|htmlspecialchars}{if $d.ownerid==0 || $d.dni<0} - {$d.owner_nick}{/if}'>{$d.descr|htmlspecialchars|default:"b.d."}</a></td>
	<td class='z'><a href='http://fs.i32.pl/s/i/domy/dom_{$d.id}.jpg' target='_blank' title='{$d.descr|htmlspecialchars}{if $d.ownerid==0 || $d.dni<0} - {$d.owner_nick}{/if}' class='domzdj'><img src='/engine/fsdomy/s/img/1320245013_lphoto.png' width='16' height='16' alt='Zdjecie' /></a></td>
<td>{if $d.ownerid==0 || $d.dni<0}&nbsp;{else}{$d.owner_nick}{/if}</td>
<td>{if $d.ownerid==0 || $d.dni<0}{if $d.koszt==0}SMS{else}{$d.koszt}{/if}{else}&nbsp;{/if}</td>
</tr>
{/foreach}
</tbody>
</table>
</fieldset>

