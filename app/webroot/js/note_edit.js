$(function(){
  var w_width = $('#draw_window').width();
  var w_height = $('#draw_window').height();
  var cam_x = 0;
  var cam_y = 0;
  var mag = 1.0; /* 表示倍率 */
  var tile_w = 256;
  var tile_h = 256;
  
  var mx = 0;
  var my = 0;
  var drag = false;
  var fileNameData;
  //console.log('width:'+w_width);
  //console.log('height:'+w_height);
  
  function load_img(url, id){
    //imgPreloaderオブジェクトを作る、名前は任意
    var imgPreloader=new Image();
    //onloadイベントハンドラ追加
    imgPreloader.onload=function() {
      //ロード完了で画像を表示
      $('#'+id).attr({'src':url});
    }
    //重要、最後に書く
    imgPreloader.src=url;
  }
  
  function draw() {
    var ost_x = ((cam_x % tile_w) + tile_w) % tile_w;
    var ost_y = ((cam_y % tile_h) + tile_h) % tile_h;
    var tile_x = Math.floor(((w_width + ost_x) + tile_w - 1) / tile_w);
    var tile_y = Math.floor(((w_height + ost_y) + tile_h - 1) / tile_h);
    var x;
    var y;
    var t_ost_x = Math.floor(cam_x / tile_w);
    var t_ost_y = Math.floor(cam_y / tile_h);
    var pre_load_size = 2;
    var generated_meta_info = [];
    
    var url = location.href; 
    var parameters = url.split('/');
    var dir_id = parameters[parameters.length - 1];
    var imgName = $('#file_name_data').attr('value');
    if (imgName != null && imgName != undefined) var imgArray = imgName.split('.');
    var id_prefix = 'tile';
    var preload_id_prefix = 'preload_tile';
    if (dir_id.match(/^\d+$/)) {
        var file_prefix = '/read/image/' + dir_id + '/' + imgArray[0];
    } else {
        var file_prefix = '/read/test.bmp';
    }
    
    var lft_x = t_ost_x - 1;
    var lft_y = t_ost_y - 1;
    var rght_x = tile_x + t_ost_x;
    var rght_y = tile_y + t_ost_y;
    
    var htm = '';
    for (y = 0 ; y < tile_y ; y++){
      for (x = 0 ; x < tile_x ; x++){
        var w = tile_w;
        var h = tile_h;
        if (x * tile_w - ost_x < 0){
          w = tile_w - ost_x;
        }
        if (y * tile_h - ost_y < 0){
          h = tile_h - ost_y;
        }
        if (x * tile_w + w - ost_x > w_width){
          w = w_width - x * tile_w + ost_x;
        }
        if (y * tile_h + h - ost_y > w_height){
          h = w_height - y * tile_h + ost_y;
        }
        if (w < 0 || h < 0){
          continue;
        }
        var tile_pos_x = (tile_w*x-ost_x);
        var tile_pos_y = (tile_h*y-ost_y);
        htm += gen_img_tag(id_prefix, (y + t_ost_y), (x + t_ost_x), preload_id_prefix, w, h, tile_pos_y, tile_pos_x);
        generated_meta_info.push({prefix:id_prefix, y:(y + t_ost_y), x: (x + t_ost_x)});
      }
    }
    $('#draw_window').html(htm);

    htm = '';
    for (p = 0 ; p < pre_load_size ; p++){
      for (x = 0 ; x < tile_x ; x++){
        var pre_y = t_ost_y -1 - p;
        var pre_x = x + t_ost_x;
        htm += gen_img_tag(preload_id_prefix, pre_y, pre_x);
        generated_meta_info.push({prefix:preload_id_prefix, y:pre_y, x: pre_x});
        pre_y = tile_y + t_ost_y + p;
        pre_x = x + t_ost_x;
        htm += gen_img_tag(preload_id_prefix, pre_y, pre_x);
        generated_meta_info.push({prefix:preload_id_prefix, y:pre_y, x: pre_x});
      }
      for (y = - pre_load_size ; y < tile_y + pre_load_size ; y++){
        var pre_y = y + t_ost_y;
        var pre_x = t_ost_x -1 - p;
        htm += gen_img_tag(preload_id_prefix, pre_y, pre_x);
        generated_meta_info.push({prefix:preload_id_prefix, y:pre_y, x: pre_x});
        pre_y = y + t_ost_y;
        pre_x = tile_x + t_ost_x + p;
        htm += gen_img_tag(preload_id_prefix, pre_y, pre_x);
        generated_meta_info.push({prefix:preload_id_prefix, y:pre_y, x: pre_x});
      }
    }
    $('#pre_load_area').html(htm);
    
    generated_meta_info.forEach(function (o){
      combine_fpath_id(o.prefix, file_prefix, o.y , o.x);
    });
  };
  function gen_img_tag(id_prefix, tile_y, tile_x, preload_id_prefix = '', width = 0, height = 0, pos_y = 0, pos_x = 0){
    if ((tile_x < 0 || tile_y < 0) && width == 0 && height == 0 ){
       return '';
    }
    var htm = '';
    var id = id_prefix + '_' + tile_y + '_' + tile_x;
    var src = '';
    if ($('#' + id).length && $('#' + id).attr('src') != undefined && $('#' + id).attr('src').length){
      src = $('#' + id).attr('src');
    }
    if (width != 0 && height != 0){
      var preload_id = preload_id_prefix + '_' + tile_y + '_' + tile_x;
      var img_style = 'pointer-events: none;';
      if (pos_x < 0){
        pos_x = 0;
        img_style += 'right:0;'
      }
      if (pos_y < 0){
        pos_y = 0;
        img_style += 'bottom:0;'
      }
      if (img_style.length){
        img_style = 'style="position:absolute;' + img_style +'"';
      } 
      if (src.length == 0 && $('#' + preload_id).length && $('#' + preload_id).attr('src') != undefined && $('#' + preload_id).attr('src').length){
        src = $('#' + preload_id).attr('src');
      }
      if (src.length){
        src = ' src="' + src + '"';
      }
      htm += '<div style="width:' + width + 'px;height:' + height + 'px;position:absolute;left:'+ pos_x +'px;top:'+ pos_y +'px;overflow:hidden;">';
      htm += '<img id="' + id + '" width="'+ tile_w + '"height="'+ tile_h +'"' + img_style + src +' >\n';
      htm += '</div>';
    } else {
      if (src.length){
        src = ' src="' + src + '"';
      }
      htm += '<img id="' + id + '"' + src +' >\n';
    }
    return htm;
  }
  function combine_fpath_id(id_prefix, fpath_prefix, tile_y, tile_x){
    if (tile_x < 0 || tile_y < 0){
       return;
    }
    var id = id_prefix + '_' + tile_y + '_' + tile_x;
    var url = fpath_prefix + '_y' + tile_y + '_x' + tile_x + '.png';
    if (($('#' + id).attr('src')) != undefined && ($('#' + id).attr('src')).length){
      return;
    }
    load_img(url, id);
  }
  $('#draw_window').mousedown(function (e){
    mx = e.pageX - this.offsetLeft;
    my = e.pageY - this.offsetTop;
    drag = true;
  });
  
  $('#draw_window').mousemove(function (e){
    var p_mx = mx;
    var p_my = my;
    mx = e.pageX - this.offsetLeft;
    my = e.pageY - this.offsetTop;
    if (drag){
      cam_x += p_mx - mx;
      cam_y += p_my - my;
      draw();
    }
  });
  $('#draw_window').mouseup(function (e){
    mx = e.pageX - this.offsetLeft;
    my = e.pageY - this.offsetTop;
    drag = false;
  });
  $('#draw_window').mouseleave(function (e){
    mx = 0;
    my = 0;
    drag = false;
  });
  var timer = false;
  $(window).resize(function() {
      if (timer !== false) {
          clearTimeout(timer);
      }
      timer = setTimeout(function() {
         w_width = $('#draw_window').width();
         w_height = $('#draw_window').height();
         draw();
         //console.log('width:'+w_width);
         //console.log('height:'+w_height);
          // 何らかの処理
      }, 200); // リサイズ中かどうか判定する時間、ms
  });
  draw();
});