{include file="html.top.tpl"}
<div align="center" style="padding-left: 40px; padding-right: 40px">
<div class="info">Podczas szukania multi kont zwróć szczególną uwagę na datę ostatniego logowania oraz rejestracji. Pamiętaj, że SERIAL w SA-MP nie jest stworzony z myślą o unikalności i niektóre mogą się powtarzać bardzo często.</div>
</div>
<br/><br/>
<form method='POST'>
<fieldset class='crit'>
    <legend>Kryteria szukania</legend>
    
    <p><label><input type='text' name='nick' id='nick' /> nick gracza</label></p>
    <p><label><input type='text' name='serial' id='serial' /> serial gracza</label></p>
    <p><label><input type='text' name='ip' id='ip' /> ip gracza</label></p>
	<p><label><select name="typ">
	<option value="1" selected="selected">OR</option>
	<option value="0">AND</option>
	</select>  typ przeszukiwania</label></p>
    <input type='submit' value='&raquo;' />
</fieldset>
</form>

<table class='full' id='bany'>
<thead>
<tr>
    <th>ID</th>
	<th>Nick</th>
    <th>Ostatnie IP</th>
	<th>Rejestracja IP</th>
	<th>Ostatni SERIAL</th>
	<th>Rejestracja SERIAL</th>
	<th>Data rejestracji</th>
	<th>Ostatnio aktywny</th>
	<th>Respekt</th>
	<th>Czas gry</th>
    <th>Skill</th>
</tr>
</thead>
<tbody>
{foreach from=$lista item=b}
{if (!$ip OR $b.ip_last==$ip) AND (!$serial OR $b.serial_last==$serial)}
<tr class='bid{$b.id}'>
    <td class='banid'>{$b.id}</td>
	<td>{$b.nick}</td>
	<td>{$b.ip_last}</td>
	<td>{$b.ip_registered}</td>
	<td>{$b.serial_last}</td>
	<td>{$b.serial_registered}</td>
	<td>{$b.datetime_registered}</td>
	<td>{$b.datetime_last}</td>
	<td>{$b.respect}</td>
	<td>{$b.minut}</td>
	<td>{$b.skill}</td>
</tr>
{/if}
{/foreach}
</tbody>
</table>
<script>{literal}
$(document).ready(function() {
    var banTable=$('table#bany').dataTable({
	"bPaginate": false,
	"oLanguage": { "sUrl": "/s/dataTables.polish.txt" }
	
	});
} );

{/literal}</script>


{include file="html.bottom.tpl"}