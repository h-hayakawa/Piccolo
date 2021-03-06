<?php
/**
 * Static content controller.
 *
 * This file will render views from views/pages/
 *
 * CakePHP(tm) : Rapid Development Framework (https://cakephp.org)
 * Copyright (c) Cake Software Foundation, Inc. (https://cakefoundation.org)
 *
 * Licensed under The MIT License
 * For full copyright and license information, please see the LICENSE.txt
 * Redistributions of files must retain the above copyright notice.
 *
 * @copyright     Copyright (c) Cake Software Foundation, Inc. (https://cakefoundation.org)
 * @link          https://cakephp.org CakePHP(tm) Project
 * @package       app.Controller
 * @since         CakePHP(tm) v 0.2.9
 * @license       https://opensource.org/licenses/mit-license.php MIT License
 */

App::uses('AppController', 'Controller');

/**
 * Static content controller
 *
 * Override this controller by placing a copy in controllers directory of an application
 *
 * @package       app.Controller
 * @link https://book.cakephp.org/2.0/en/controllers/pages-controller.html
 */
class PagesController extends AppController {

/**
 * This controller does not use a model
 *
 * @var array
 */
  public $uses = array();

/**
 * Displays a view
 *
 * @return CakeResponse|null
 * @throws ForbiddenException When a directory traversal attempt.
 * @throws NotFoundException When the view file could not be found
 *   or MissingViewException in debug mode.
 */
  public function display() {
    $path = func_get_args();

    $count = count($path);
    if (!$count) {
      return $this->redirect('/');
    }
    if (in_array('..', $path, true) || in_array('.', $path, true)) {
      throw new ForbiddenException();
    }
    $page = $subpage = $title_for_layout = null;

    if (!empty($path[0])) {
      $page = $path[0];
    }
    if (!empty($path[1])) {
      $subpage = $path[1];
    }
    if (!empty($path[$count - 1])) {
      $title_for_layout = Inflector::humanize($path[$count - 1]);
    }
    $this->set(compact('page', 'subpage', 'title_for_layout'));

    try {
      $this->render(implode('/', $path));
    } catch (MissingViewException $e) {
      if (Configure::read('debug')) {
        throw $e;
      }
      throw new NotFoundException();
    }
  }
  public function image() {
      $filepath = Router::url();
      $len = strlen($filepath);
      $filepath = substr($filepath, 6, $len);
      
      $fileInfo = explode('/', $filepath);
      $fileName = $fileInfo[count($fileInfo) - 1];
      $tileInfo = explode('_', $fileName);
      $tileInfo[count($tileInfo) - 1] = explode('.', $tileInfo[count($tileInfo) - 1])[0];
      
      $gen = $fileInfo[count($fileInfo) - 2];
      $id = $fileInfo[count($fileInfo) - 3];
      
      $orgBmp = ROOT.DS.APP_DIR.'/tmp/files/image/'.$id.'/'.$gen.'/raw/'.$id.'.bmp';
      $this->layout = false;
      $this->render(false);
      $imgFile = COMMON_FILE_PATH . $filepath;
      if (!file_exists($imgFile)){
          $pi = null;
          $pipeDir = PIPE_ROOT_DIR.$id;
          if (!file_exists($pipeDir)){
              mkdir($pipeDir, 0777, true);
          }
          $inPipe = $pipeDir.'/input';
          if (!file_exists($pipeDir.'/input')){
              posix_mkfifo($inPipe, '0500');
              $exe = ROOT.DS.'bin/core_app '. "$inPipe" . ' ' ." > /dev/null &";
              $pi = fopen($inPipe, 'w+');
              exec($exe);
              fwrite($pi, "load_bmp ".$orgBmp."\n");
              fflush($pi);
          } else {
              $pi = fopen($inPipe, 'w+');
          }
          $tilePipeDir = PIPE_ROOT_DIR.$id.'/'.$gen;
          if (!file_exists($tilePipeDir)){
              mkdir($tilePipeDir, 0777, true);
          }
          $tilePipe = PIPE_ROOT_DIR.$id.'/'.$gen.'/'.$id.'_'.$tileInfo[1].'_'.$tileInfo[2].'_'.$tileInfo[3];
          if (file_exists($tilePipe)){
              unlink($tilePipe);
          }
          posix_mkfifo($tilePipe, '0500');
          $rspPipe = fopen($tilePipe, 'w+');
          $cmd = "output_tile ".ROOT.DS.APP_DIR.'/tmp/files/image/'.$id.'/'.$gen.'/'.$tileInfo[0]." ".$tileInfo[1]." ".$tileInfo[2]." ".$tileInfo[3]." ". $tilePipe ."\n";
          
          while (1){
            fwrite($pi, $cmd);
            fflush($pi);
            $r = fgets($rspPipe);
            if ($r == "done\n"){
              break;
            }
            if ($r == "err\n"){
              break;
            }
            if ($r == "retry\n"){
              ;
            }
            usleep(300000);
          }
          
          fclose($pi);
          fclose($rspPipe);
          unlink($tilePipe);
      }
      $finfo = new finfo(FILEINFO_MIME_TYPE);
      $mimeType = $finfo->file($imgFile);
      header('Content-type: ' . $mimeType . '; charset=UTF-8');
      readfile($imgFile);
  }
}
