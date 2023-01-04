<?php
include "config.php";
//$url=$_REQUEST['url'];
//$sql = "SELECT `id`, `path`, `Status`, `match_id`, `isTeamBase` WHERE `Status`=2 LIMIT 1";
$sql = "SELECT `id`, `path`, `Status`, `match_id` FROM `url_service` WHERE `Status`=2 LIMIT 1";

if ($result = mysqli_query($conn, $sql)) {
 // echo "New record created successfully";


if (mysqli_num_rows($result) > 0) {
  // output data of each row
  while($row = mysqli_fetch_assoc($result)) {
	  echo json_encode($row);
   // echo "id: " . $row["id"]. " - path: " . $row["path"]. " " . $row["Status"]. "<br>";
  }
} else {
 // echo "0 results";
$arr_no_value = array("id"=>0);

echo json_encode($arr_no_value);
}

} else {

}
mysqli_close($conn);

?>