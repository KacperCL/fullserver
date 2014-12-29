{* Smarty *}
{assign var=p value=$RDB->pobierz_wyniki("select p.id,p.nick,p.last_skin,p.respect,p.skill,p.gamep,p.datetime_last,p.datetime_registered,p.ban_count,p.kick_count,p.login_count,p.kill_count,p.teamkill_count,p.death_count,p.suicide_count,p.bank_money,p.wallet_money,p.hitman_prize,IF(p.level>4,0,p.level) level,(p.vip>=NOW()) vip, (select id from fs_houses where ownerid=$pid and paidTo>=DATE(NOW())) dom, (select g.id from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_id, (select g.color from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_color, (select g.name from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_name, (select g.tag from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_tag, (select pig.rank from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_rank, (SELECT date_end FROM fs_bans WHERE player_banned = $pid AND date_end > NOW() AND date_created < NOW() LIMIT 1) ban_datetime FROM fs_players p WHERE p.id=$pid")}
{if $p.id>0}
{strip}
{if $format eq 'json'}
{json_encode($p)}
{elseif $format eq 'xml'}
{header('Content-type: application/xml; charset="utf-8"')}
{xml toXml=$p}
{elseif $format eq 'yaml'}
{* {yaml_emit($p)} *}temporary error
{else}
unknown format type
{/if}
{/strip}
{/if}
