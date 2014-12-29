<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="pl" >
<head>
    <title>{$html_title}</title>
    <meta http-equiv="Content-type" content="text/html; charset=UTF-8" />
    <meta http-equiv="content-language" content="PL" />
    <link rel='stylesheet' href='/s/css/content.css?data=20130623' type='text/css' />

	<link rel='stylesheet' href='/s/css/fullserver/jquery-ui-1.8.14.custom.css' type='text/css' />
	<script src='/s/js/jquery-1.5.1.min.js' type='text/javascript'></script>
	<script src='/s/js/jquery-ui-1.8.14.custom.min.js' type='text/javascript'></script>
	<script src='/s/js/jquery.autocomplete.min.js' type='text/javascript'></script>
	<script src='/s/js/whcookies.js' type='text/javascript'></script>
    {foreach from=$html_js item=js}
	<script type='text/javascript' src='{$js}'></script>
	{/foreach}
    {foreach from=$html_css item=css}
	<link rel='stylesheet' href='{$css}' type='text/css' />{/foreach}
    <link rel="icon" type="image/x-icon" href="/favicon.ico">
{literal}
<script>
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');

  ga('create', 'UA-40915694-3', 'i128.pl');
  ga('send', 'pageview');

</script>
{/literal}
</head>
<body class="dashborad">
<div id='header'>
<div class='logo'>
<h1><a href='http://fullserver.eu/'><img src="http://fullserver.eu/public/style_images/16_0IqjaKI.png"></a></h1>
</div>
<form class='szukajgracza' onsubmit='return false;'>
<span class="right" id="search_wrap">
<span style="" id="search_icon" class="choice ipbmenu clickable"></span>
<input type='text' name='nick' id='nick' placeholder="szukaj gracza"/>
</span>
</form>
</div>
{include file="_menu.tpl"}
<div id='main'>
<hr style="border: 0px solid gray; height: 0px; width: 0px; margin-top: 62px ;">