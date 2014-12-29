{* Smarty *}

{include file="html.top.tpl"}

<table class='full' id='aa'>
<thead>
  <tr>
	<th>ID</th>
	<th>Nick</th>
	<th align='right'>Nadanych banow (lacznie z usunietymi)</th>
  </tr>

</thead>
<tbody>
{foreach from=$bany item=w key=osoba}

  <tr class="level{$w.level}">
	<td align='right' width='7%'>{$w.id|intval}</td>
	<td width='33%'>{$osoba|htmlspecialchars}</td>
	<td align='right'>{$w.bany|intval}</td>	
  </tr>

{/foreach}
</tbody>
</table>
<br/>

<script>{literal}

var k_abs=$("input[name=k_abs]:checked").length;

$(document).ready(function() {
    $('table#aa').dataTable({
	"bPaginate": false,
	"oLanguage": { "sUrl": "/s/dataTables.polish.txt" },
	"aoColumnDefs": [
	    {	"bSearchable": false, 
		"bUseRendered": false,
		"fnRender": function ( oObj ) {
			var m=oObj.aData[oObj.iDataColumn];
			if (m==0) return "-";
			if(!k_abs) {
			    var podstawa=((oObj.iDataColumn==6?3:(oObj.iDataColumn==7?3:1))*420);
			    if (m>=podstawa) return "0m";
			    m=podstawa-m;
			    return m+'m';
			}
			return Math.floor(m/60)+'h'+(m%60)+'m';
			
		 },
	        "aTargets": [ "minuty" ] }
	    
	]
	
	});
} );

{/literal}</script>

{include file="html.bottom.tpl"}
