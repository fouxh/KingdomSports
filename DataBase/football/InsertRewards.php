<?php
include "config.php";
$p_id=$_REQUEST['PID'];
$Gold=$_REQUEST['Gold'];
$Gem=$_REQUEST['Gem'];
$Trophies=$_REQUEST['Trophies'];
$Goals=$_REQUEST['Goals'];
$Assist=$_REQUEST['Assist'];
$Win=$_REQUEST['Win'];
$Lose=$_REQUEST['Lose'];
$Draw=$_REQUEST['Draw'];
//InsertRewards.php?PID=ghghg&Gold=1&Gem=2&Trophies=5&Goals=4&Assist=6&Win=5&Lose=11&Draw=4
$sql = "INSERT INTO `player_info`( `PlayerID`, `Gold`, `Gem`, `Trophies`, `Goals`, `Assist`, `Win`, `Lose`, `Draw`) VALUES ('".$p_id."',".$Gold.",".$Gem.",".$Trophies.",".$Goals.",".$Assist.",".$Win.",".$Lose.",".$Draw.")";
$val1="ahmed";
$val2=2;

if (mysqli_query($conn, $sql)) {
  //echo "New record created successfully";
} else {
  //echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}

mysqli_close($conn);
?>