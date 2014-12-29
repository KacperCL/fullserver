{* Smarty *}

{include file="_header.tpl"}
<script type="text/javascript">
window.onload=function() {
var data=location.search;
if(data) {
    data=location.search.substring(1); // remove the '/'
    frames[2].location = 'http://fs.i32.pl/engine/fsdomy/'+data;
    }
};
</script>
<div class='tabs'>
	<ul>
		<li><a href='#gamep'>Doładuj GamePoints</a></li>
		<li><a href='#haslo'>Zresetuj hasło na serwerze</a></li>
		<li><a href='#domki'>Lista domków na serwerze</a></li>
	</ul>
	<div id='gamep' style="height:920px" class="platnosci">
		<iframe src='/engine/gamep' scrolling="no" frameborder='0' style="width:100%;height:100%"><p>Ups! Wystąpił nieoczekiwany problem podczas ładowania silnika... :/</p></iframe>
	</div>
	<div id='haslo' style="height:420px" class="platnosci">
		<iframe src='/engine/fshaslo' scrolling="no" frameborder='0' style="width:100%;height:100%"><p>Ups! Wystąpił nieoczekiwany problem podczas ładowania silnika... :/</p></iframe>
	</div>
	<div id='domki' style="height:850px" class="platnosci">
		<iframe src='/engine/fsdomy' scrolling="yes" frameborder='0' style="width:100%;height:100%"><p>Ups! Wystąpił nieoczekiwany problem podczas ładowania silnika... :/</p></iframe>
	</div>
</div>

{include file="_footer.tpl"}