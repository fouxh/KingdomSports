<?php
include "config.php";
$clanid=$_REQUEST['clanid'];
//localhost/football/ClanInfo.php?clanid=6
$sqlClans = "SELECT * FROM `clans` WHERE `clan_id`='$clanid'";
$result = mysqli_query($conn, $sqlClans);

if (mysqli_num_rows($result) > 0)
  {
  $result_array["data"]=mysqli_fetch_all($result, MYSQLI_ASSOC);
  echo json_encode($result_array);
  }
else
  {
  mysqli_close($conn);
  }
mysqli_close($conn);
?>
