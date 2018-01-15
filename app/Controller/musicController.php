<?php

App::uses('AppController', 'Controller');

class MusicController extends AppController {
    public function beforeFilter() {
        parent::beforeFilter();
        //$this->Auth->allow();
    }

    public function index() {
        $imageList = $this->getImage();
        $this->set(compact('imageList'));
        $this->layout = '';
    }
    
    public function note_edit($id){
        $outputDir = ROOT.DS.APP_DIR.'/tmp/files/image/'.$id.'/';
        
        if (count(glob($outputDir."*.png")) == 0){
            $pipeDir = PIPE_ROOT_DIR.$id;
            $input_fpath = ROOT.DS.APP_DIR.'/tmp/origFiles/image/'.$id.'/'.$id.'.bmp';
            $output_fpath_prefix = $outputDir.$id;
            if (!file_exists($pipeDir)){
                mkdir($pipeDir, 0777, true);
            }
            
            if (file_exists($pipeDir.'/input')){
                unlink($pipeDir.'/input');
            }
            if (file_exists($pipeDir.'/output')){
                unlink($pipeDir.'/output');
            }
            $inPipe = $pipeDir.'/input';
            $outPipe = $pipeDir.'/output';
            
            posix_mkfifo($inPipe, '0500');
            posix_mkfifo($outPipe, '0500');
            $exe = ROOT.DS.'bin/core_app '. "$inPipe" . ' ' . "$outPipe"." > /dev/null &";
            $pi = fopen($inPipe, 'w+');
            $po = fopen($outPipe, 'r+');
            exec($exe);
            
            fwrite($pi, "load_bmp ".$input_fpath."\n");
            fwrite($pi, "output_tile_all ".$output_fpath_prefix."\n");
            
            fwrite($pi, "exit\n");
            fclose($pi);
            
            $s = fgets($po);
            //pr($s);
        }
        $this->set('imageName', $id);
        $this->layout = '';
    }
    
    public function add()
    {
        if ($this->request->is('post')) {
            if ($this->Music->save($this->request->data)) {
                $id = $this->Music->getLastInsertID();
                $directoryPath = ROOT.DS.APP_DIR.'/tmp/files/image/'.$id;
                if (mkdir($directoryPath, 0777, true)) {
                    $this->imageConversion($id, $this->request->data['Music']['image']['name']);
                }
            }
            $this->Flash->error(__('Unable to save the Image.'));
        }
    }
    
    public function getImage($id = 0)
    {
        if ($id != 0) {
            $imageData = $this->Music->find('first',array(
                'conditions' => array(
                    'id' => $id,
                ),
            ));
            return $imageData;
        } else {
            $imageList = $this->Music->find('list', array(
                'fields' => array('id', 'image')
                )
            );
            return $imageList;
        }
    }
    
    public function imageConversion($id = 0, $fname = '')
    {
        $flg = 1;
        $spec = array( 
            0 => array("pipe", "r"), 
            1 => array("pipe", "w"),  
            2 => array("file", ROOT.DS.APP_DIR."/tmp/error-output.txt ", "a") 
        ); 
        $input_fpath_prefix = ROOT.DS.APP_DIR.'/tmp/origFiles/image/';
        $output_fpath_prefix = ROOT.DS.APP_DIR.'/tmp/files/image/';            
        $rootName = explode('.', $fname);
        $reFilename =  $id.'.bmp';
        $tileFileName = $id;
        $exe = 'convert '. "$fname" . ' -type truecolor ' . "$reFilename";

        $process = proc_open($exe, $spec, $pipes, ROOT.DS.APP_DIR.'/tmp/origFiles/image/'.$id, null);
        if (is_resource($process)) {
            fclose($pipes[0]);
            fclose($pipes[1]);;
            $flg = proc_close($process);
        }
        
        if ($flg == 0) {
          /*
            $flg = 1;
            $input_fpath = $input_fpath_prefix.$id.DS.$reFilename;
            $output_fpath = $output_fpath_prefix.$id.DS.$tileFileName;    
            $exe = ROOT.DS.'bin/bmp2tile '. "$input_fpath" . ' ' . "$output_fpath";
            
            
            $process = proc_open($exe, $spec, $pipes, ROOT.DS.'src', null);
            if (is_resource($process)) {
                fclose($pipes[0]);
                fclose($pipes[1]);;
                $flg = proc_close($process);
            }
          */
            if ($flg == 0) {
                $this->Flash->success(__('The Image has been saved and combined'));
                $this->redirect(array('action' => 'index'));
            } else {
                $this->Flash->error(__('Unable to combine the Image.'));
                $this->redirect(array('action' => 'index'));
            }
        }
    }
}
