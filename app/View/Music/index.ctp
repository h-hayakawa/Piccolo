<!DOCTYPE html>
<html lang="ja" style="height:100%;">
    <head>
        <meta charset="utf-8">
            <title>NULLさんの実験室</title>
            <meta name="viewport" content="width=device-width,initial-scale=1" />
            <link rel="stylesheet" href="/css/bootstrap.css">
    </head>
    <body style="height:100%;">
        <nav class="navbar navbar-inverse" style="margin-bottom: 0px;">
            <div class="container-fluid">
                <div class="nav-header">
                    <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
                        <span class="sr-only">Toggle navigation</span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                    </button>
                    <a class="navbar-brand" href="/">top</a>
                </div><!-- nav-header -->
                <div id="navbar" class="navbar-collapse collapse in" area-expanded="true">
                    <ul class="nav navbar-nav navbar-right">
                        <li><a href="">お知らせ</a></li>
                        <li><a href="">お問い合わせ</a></li>
                        <li><a href="">このサイトについて</a></li>
                    </ul>
                    <form class="navbar-form navbar-right">
                        <input type="text" class="form-control" placeholder="検索">
                    </form>
                </div><!-- navbar -->
            </div><!-- container-fluid -->
        </nav>
        
        <div class="main-content" style="margin-left:20px;height:calc( 100% - 52px );width:calc( 100% - 35px );">
            <div class="row" style="height:100%;">
                <div class="col-md-3 sidebar">
                    side
                </div><!--/.sidebar-->
                <div id="add_image" class="col-md-9 content-area" style="border:solid 1px;height:100%; padding:0;">
                    <?php echo $this->Form->create('Music', array('type' => 'file', 'url' => 'add')); ?>
                        <?php echo $this->Form->input('Music.image', array('type' => 'file')); ?>
                        <?php echo $this->Form->input('Music.image_dir', array('type' => 'hidden')); ?>
                    <?php echo $this->Form->end('Submit'); ?>
                    
                        <?php if ($imageList): ?>
                            <h4>Image List</h4>
                            <div class="table-responsive">                                
                                <table class="table table-condensed">
                                    <thead>
                                        <tr>
                                            <th>#</th>
                                            <th>Image</th>
                                            <th>Original Flie</th>
                                            <th>Combined FileDir</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php foreach ($imageList as $key => $value): ?>
                                        <tr class="active">
                                            <th><?php echo $key ?></th>
                                            <th><?php echo $value ?></th>
                                            <th><?php echo (APP_DIR.'/tmp/origFiles/image/'.$key.DS.$value) ?></th>
                                            <th><?php echo (APP_DIR.'/tmp/files/image/'.$key) ?></th>
                                            <th><?php 
                                                    echo $this->Form->postLink(__('Edit'), 
                                                        array('action' => 'note_edit', $key),
                                                        array('class' => 'btn btn-primary')
                                                    );
                                                ?>
                                            </th>
                                        </tr>
                                        <?php endforeach; ?>
                                    </tbody>
                                </table>
                            </div>
                        <?php endif; ?>
                </div><!--/.content-area-->

            </div>
        </div><!--/.main-content-->
    
    </body>
    <script type="text/javascript" src="/js/jquery.1.12.4.min.js"></script>
    <script type="text/javascript" src="/js/bootstrap.min.js"></script>
    <script type="text/javascript" src="/js/note_edit.js"></script>
</html>