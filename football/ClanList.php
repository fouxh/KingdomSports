<?php
include "config.php";
//localhost/football/Clanlist.php?
$sqlClans = "SELECT * FROM `clans` WHERE 1";
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
