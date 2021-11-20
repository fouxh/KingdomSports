<?php
include "config.php";
$p_id=$_REQUEST['id'];
$gold=$_REQUEST['gold'];
$gem=$_REQUEST['gem'];
$Trophies=$_REQUEST['Trophies'];
$Goals=$_REQUEST['Goals'];
$Assist=$_REQUEST['Assist'];
$Win=$_REQUEST['Win'];
$Lose=$_REQUEST['Lose'];
$Draw=$_REQUEST['Draw'];
//test2.php?id=ghghg&gold=1&gem=2&Trophies=5&Goals=4&Assist=6&Win=5&Lose=11&Draw=4
$sql = "INSERT INTO `player_info`( `id_pl`, `gold`, `gem`, `Trophies`, `Goals`, `Assist`, `Win`, `Lose`, `Draw`) VALUES ('".$p_id."',".$gold.",".$gem.",".$Trophies.",".$Goals.",".$Assist.",".$Win.",".$Lose.",".$Draw.")";
$val1="ahmed";
$val2=2;

if (mysqli_query($conn, $sql)) {
  //echo "New record created successfully";
} else {
  //echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}

mysqli_close($conn);
?>