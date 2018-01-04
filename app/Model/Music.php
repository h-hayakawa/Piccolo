<?php
    class Music extends AppModel
    {
        public $actsAs = array(
            'Upload.Upload' => array(
                'image' => array(
                    'fields' => array(
                        'dir' => 'image_dir'
                    ),
                    'path' => '{ROOT}tmp{DS}origFiles{DS}{field}{DS}',
                    'mimetypes' => array('image/jpeg', 'image/gif', 'image/png', 'image/bmp'),
                    'extensions' => array(
                        'jpg', 'jpeg', 'JPG', 'JPEG', 'gif', 'GIF', 'png', 'PNG', 'bmp', 'BMP',
                        'aai', 'AAI', 'avs', 'AVS', 'art', 'ART', 'bgr', 'BGR', 'cals', 'CALS',
                        'cin', 'CIN', 'cur', 'CUR', 'pcx', 'PCX', 'dpx', 'DPX', 'pdf', 'PDF',
                        'ps', 'PS', 'ept', 'EPT', 'exr', 'EXR', 'fax', 'FAX', 'fits', 'FITS',
                        'fts', 'FTS', 'tiff', 'TIFF', 'hdr', 'HDR', 'hrz', 'HRZ', 'tga', 'TGA',
                        'icon', 'ICON', 'ipl', 'IPL', 'jp2', 'JP2', 'mpeg', 'MPEG', 'map', 'MAP',
                        'mat', 'MAT', 'miff', 'MIFF', 'mono', 'MONO', 'mpc', 'MPC', 'msl', 'MSL',
                        'svg', 'SVG', 'mtv', 'MTV', 'mvg', 'MVG', 'otb', 'OTB', 'palm', 'PALM',
                        'pnm', 'PNM', 'pcd', 'PCD', 'pcl', 'PCL', 'pict', 'PICT', 'pdb', 'PDB',
                        'pfm', 'PFM', 'xpm', 'XPM', 'psd', 'PSD', 'sun', 'SUN', 'sgi', 'SGI',
                        'txt', 'TXT', 'vicar', 'VICAR', 'vid', 'VID', 'viff', 'VIFF', 'wbmp', 'WBMP',
                        'xbm', 'XBM', 'xpm', 'XPM', 'xwd', 'XWD', 'yuv', 'YUV'
                    ),
                )
            )
        );
    }