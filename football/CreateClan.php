<?php
include "config.php";
$clanname=$_REQUEST['clanname'];
$logo_index=$_REQUEST['logo_index'];
$trophies=$_REQUEST['trophies'];
$type=$_REQUEST['type'];
$required_trophies=$_REQUEST['required_trophies'];
$description=$_REQUEST['description'];
//$PlayerID=$_REQUEST['PlayerID'];
$json_array;
//localhost/football/CreateClan.php?clanname=dragons&logo_index=2&type=1&trophies=200&required_trophies=100&description=awesomeclan

$sqlClan = "SELECT `clan_name` FROM `clans` WHERE `clan_name` = '".$clanname."'";
$result = mysqli_query($conn, $sqlClan);


if (mysqli_num_rows($result) > 0)
  {
  $json_array["message"]="Clan Already Exists";
  echo json_encode($json_array);
  }
else
  {
  $sqlClan ="INSERT INTO `clans`(`clan_name`, `logo_index`,`trophies`, `type-index`, `req_trophies`, `desc`)
             VALUES ('".$clanname."',
             '$logo_index',
             '$trophies',
             '$type',
             '$required_trophies',
             '".$description."')";

  mysqli_query($conn, $sqlClan);
  $json_array["message"]="Clan Created Successfully";
  echo json_encode($json_array);

  }
mysqli_close($conn);
?>
