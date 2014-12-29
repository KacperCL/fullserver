

(function($) {

jQuery.extend(jQuery.validator.messages, {
    required: "To pole jest wymagane.",
    remote: "Proszę o wypełnienie tego pola.",
    email: "Proszę o podanie prawidłowego adresu email.",
    url: "Proszę o podanie prawidłowego URL.",
    date: "Proszę o podanie prawidłowej daty.",
    dateISO: "Proszę o podanie prawidłowej daty (ISO).",
    number: "Proszę o podanie prawidłowej liczby.",
    digits: "Proszę o podanie samych cyfr.",
    creditcard: "Proszę o podanie prawidłowej karty kredytowej.",
    equalTo: "Proszę o podanie tej samej wartości ponownie.",
    accept: "Proszę o podanie wartości z prawidłowym rozszerzeniem.",
    maxlength: jQuery.validator.format("Nie więcej niż {0} znaków."),
    minlength: jQuery.validator.format("Minimalnie {0} znaków."),
    rangelength: jQuery.validator.format("Proszę o podanie wartości o długości od {0} do {1} znaków."),
    range: jQuery.validator.format("Proszę o podanie wartości z przedziału od {0} do {1}."),
    max: jQuery.validator.format("Proszę o podanie wartości mniejszej bądź równej {0}."),
    min: jQuery.validator.format("Proszę o podanie wartości większej bądź równej {0}.")
});
})();
																		
						    

function homebuy_submit() {
//    if (!$("#vipbuy").validate()) return false;
    $("button#submitbtn").html("Proszę czekać").attr('disabled','disabled');

    $.post('/engine/fsdomy/index.php',$("form").serializeArray(),function(data) {
        $(data).prependTo("body:first").fadeIn(1000);
	    $("#vipinfo,#homebuy,#nav").fadeOut(500);
	    $("button#submitbtn").html("Dalej").removeAttr('disabled');
	});
    
}
//$("form.validate").validate();

$(function() {
    // progrssbar
	$( ".radioset" ).buttonset();

    $(".progressbar").each(function(){
	var prc=$(this).text().replace('%','');
	$(this).text('');
        $(this).progressbar({value: 15});
    });	
    $("form.validate").each(function(){
    $(this).validate({
		rules: {
			nick: {
				required: true,
				remote: "checknick.php"
			}
		},

		submitHandler: homebuy_submit,
        ignore: ":disabled,:hidden",
        focusInvalid: false
        });
	});
    
//	$("#vipbuy").valid();
    // vipbuy
//	if ($("form.validate").length>0)
//		$("form.validate input[name=nick]").valid();
	$("a.domzdj").hover(function(e){
		url=$(this).attr('href');
		nx=e.pageX>340?(e.pageX-340):(e.pageX+32);

		ny=e.pageY-120;
		url320=url.replace(/(.*)\//,"$1/320_");

		$("#hpreview").html("<img src='"+url320+"' width='320' height='240' />").css('left',nx).css('top',ny).stop(true,true).fadeIn("fast");
	},function(){
		$("#hpreview").fadeOut("slow");
	});

	zd_zmien();

});

function zd_zmien(){
	if ($("input#domyall:checked").length) {
		$("table.domy>tbody>tr.zajety").show();
		$("table.domy>tbody>tr.gzajety").show();
		$("table.domy>tbody>tr.wolny").show();
		$("table.domy>tbody>tr.gwolny").show();
	} else if ($("input#domyw:checked").length) {
		$("table.domy>tbody>tr.zajety").hide();
		$("table.domy>tbody>tr.gzajety").hide();
		$("table.domy>tbody>tr.wolny").show();
		$("table.domy>tbody>tr.gwolny").show();
	} else if ($("input#domyz:checked").length) {
		$("table.domy>tbody>tr.zajety").show();
		$("table.domy>tbody>tr.gzajety").show();
		$("table.domy>tbody>tr.wolny").hide();
		$("table.domy>tbody>tr.gwolny").hide();
	} else if ($("input#domyg:checked").length) {
		$("table.domy>tbody>tr.zajety").hide();
		$("table.domy>tbody>tr.wolny").hide();
                $("table.domy>tbody>tr.gwolny").show();
		$("table.domy>tbody>tr.gzajety").show();

	}
}
