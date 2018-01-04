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
        $input_fpath_prefix = '../'.APP_DIR.'/tmp/origFiles/image/';
        $output_fpath_prefix = '../'.APP_DIR.'/tmp/files/image/';            
        $rootName = explode('.', $fname);
        $reFilename =  $rootName[0].'.bmp';
        $tileFileName = $id;
        $exe = 'convert '. "$fname" . ' -type truecolor ' . "$reFilename";

        $process = proc_open($exe, $spec, $pipes, ROOT.DS.APP_DIR.'/tmp/origFiles/image/'.$id, null);
        if (is_resource($process)) {
            fclose($pipes[0]);
            fclose($pipes[1]);;
            $flg = proc_close($process);
        }
        
        if ($flg == 0) {
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
