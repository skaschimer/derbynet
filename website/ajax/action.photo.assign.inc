<?php
// $_POST['repo'] - photo repository name
// $_POST['racer'] - racerid of racer whose photo should be set
// $_POST['photo'] - basename of photo file; empty if removing association
require_once('inc/photo-config.inc');
require_once('inc/events.inc');

$repo = photo_repository(isset($_POST['repo']) ? $_POST['repo'] : 'head');

if (have_permission(ASSIGN_RACER_IMAGE_PERMISSION)) {
  if (isset($_POST['photo']) && strlen($_POST['photo'])) {
    $photo_path = $repo->lookup(RENDER_CROPPED)->find_or_make_image_file($_POST['photo']);
    if (!$photo_path) {
      json_failure('nosuchfile', "Image ".$_POST['photo']." not found.");
    } else {
      if (take_action_silently('UPDATE RegistrationInfo SET '.$repo->column_name().' = :photo'
                               .' WHERE racerid = :racerid',
                               array(':photo' => $photo_path,
                                     ':racerid' => $_POST['racer']))) {
        json_success();
      } else {
        json_sql_failure($sql);  // TODO Missing sql
      }
      record_event($repo->name() == 'head' ? EVENT_RACER_PHOTO_ASSIGN : EVENT_CAR_PHOTO_ASSIGN,
                   array('racerid' => $_POST['racer']));
    }
  } else {
    if (take_action_silently('UPDATE RegistrationInfo SET '.$repo->column_name().' = \'\''
                             .' WHERE racerid = :racerid',
                             array(':racerid' => $_POST['racer']))) {
      json_success();
    } else {
      json_sql_failure($sql);  // TODO Missing sql
    }
    record_event($repo->name() == 'head' ? EVENT_RACER_PHOTO_ASSIGN : EVENT_CAR_PHOTO_ASSIGN,
                 array('racerid' => $_POST['racer']));
  }
} else {
  json_not_authorized();
}
?>