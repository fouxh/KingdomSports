<?php
include "config.php";
// Query the table to get the queue array
$return_arr = array();

$sql = "SELECT * FROM ready_match where url IS NULL";
$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) > 0) {
  // Output the queue array as a JSON response
  while($row = mysqli_fetch_assoc($result)) {
	  // echo json_encode($row['queue']);


$return_arr['id']=$row['id'];
$return_arr['map']=$row['map'];

$return_arr['message']="Map found ";
	
   //array_push($return_arr,$row['queue']);
  }
 
 
} 
else 
{
$return_arr['id']=0;
$return_arr['map']="no";
 $return_arr['message']="Not found";
}

// Close connection
mysqli_close($conn);
echo  json_encode($return_arr);
?>


