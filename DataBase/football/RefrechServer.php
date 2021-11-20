<?php

$sqlDead = "SELECT `id`, `path`, `Status`,`match_id` FROM `url_service` WHERE `Status`=3 LIMIT 1";	
//echo $sqlDead;
if ($result = mysqli_query($conn, $sqlDead)) 
{
	 while($row = mysqli_fetch_assoc($result)) {
	  //echo json_encode($row);
	  $id=$row['id'];
 ////////////////////////////////////////////
 if($id)
{
$status=$_REQUEST['status'];

$sqlupdate = "UPDATE `url_service` SET `Status`=2,`match_id`=0 WHERE `id`=".$id;	
//echo $sql;
if ($result = mysqli_query($conn, $sqlupdate)) 
{
$arr_no_value = array("path"=>"refresh");
echo json_encode($arr_no_value);
} 
else {

}

}
 //////////////////////////////////////////
  }

} 


//mysqli_close($conn);
?>