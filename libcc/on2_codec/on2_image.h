/*
 * Copyright 2007
 * On2 Technologies Incorporated
 *
 * All rights reserved.
 *
 * $Id$
 */
/*!\file on2_image.h
 * \brief Describes the On2 image descriptor and associated operations
 *
 */
#ifndef ON2_IMAGE_H
#define ON2_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*!\brief Current ABI version number
 *
 * \internal
 * If this file is altered in any way that changes the ABI, this value
 * must be bumped.  Examples include, but are not limited to, changing
 * types, removing or reassigning enums, adding/removing/rearranging
 * fields to structures
 */
//#define ON2_IMAGE_ABI_VERSION (1) /**<\hideinitializer*/


#define IMG_FMT_PLANAR     0x100  /**< Image is a planar format */
#define IMG_FMT_UV_FLIP    0x200  /**< V plane precedes U plane in memory */
#define IMG_FMT_HAS_ALPHA  0x400  /**< Image has an alpha channel componnent */


/*!\brief List of supported image formats */
typedef enum img_fmt {
    IMG_FMT_NONE,
    IMG_FMT_RGB24,   /**< 24 bit per pixel packed RGB */
    IMG_FMT_RGB32,   /**< 32 bit per pixel packed 0RGB */
#if 0
    IMG_FMT_RGB565,  /**< 16 bit per pixel, 565 */
    IMG_FMT_RGB555,  /**< 16 bit per pixel, 555 */
    IMG_FMT_UYVY,    /**< UYVY packed YUV */
    IMG_FMT_YUY2,    /**< YUYV packed YUV */
    IMG_FMT_YVYU,    /**< YVYU packed YUV */
    IMG_FMT_BGR24,   /**< 24 bit per pixel packed BGR */
    IMG_FMT_RGB32_LE, /**< 32 bit packed BGR0 */
    IMG_FMT_ARGB,     /**< 32 bit packed ARGB, alpha=255 */
    IMG_FMT_ARGB_LE,  /**< 32 bit packed BGRA, alpha=255 */
    IMG_FMT_RGB565_LE,  /**< 16 bit per pixel, gggbbbbb rrrrrggg */
    IMG_FMT_RGB555_LE,  /**< 16 bit per pixel, gggbbbbb 0rrrrrgg */
#endif
    IMG_FMT_YV12    = IMG_FMT_PLANAR | IMG_FMT_UV_FLIP | 1, /**< planar YVU */
#if 0
    IMG_FMT_I420    = IMG_FMT_PLANAR | 2,
    IMG_FMT_ON2YV12 = IMG_FMT_PLANAR | IMG_FMT_UV_FLIP | 3, /** < planar 4:2:0 format with on2 color space */
    IMG_FMT_ON2I420 = IMG_FMT_PLANAR | 4,  /** < planar 4:2:0 format with on2 color space */
#endif
} img_fmt_t; /**< alias for enum img_fmt */


/**\brief Image Descriptor */
typedef struct {
    img_fmt_t     fmt; /**< Image Format */

    /* Image storage dimensions */
    unsigned int  w;   /**< Stored image width */
    unsigned int  h;   /**< Stored image height */

    /* Image display dimensions */
    unsigned int  d_w;   /**< Displayed image width */
    unsigned int  d_h;   /**< Displayed image height */

    /* Chroma subsampling info */
    unsigned int  x_chroma_shift;   /**< subsampling order, X */
    unsigned int  y_chroma_shift;   /**< subsampling order, Y */

    /* Image data pointers. */
#define PLANE_PACKED 0   /**< To be used for all packed formats */
#define PLANE_Y   0      /**< Y (Luminance) plane */
#define PLANE_U   1      /**< U (Chroma) plane */
#define PLANE_V   2      /**< V (Chroma) plane */
#define PLANE_ALPHA 3    /**< A (Transparancy) plane */
    unsigned char *planes[4];  /**< pointer to the top left pixel for each plane */
    int      stride[4];  /**< stride between rows for each plane */

    int     bps; /**< bits per sample (for packed formats) */

    /* The following member may be set by the application to associate data
     * with this image.
     */
    void    *user_priv; /**< may be set by the application to associate data
                         *   with this image. */

    /* The following members should be treated as private. */
    unsigned char *img_data;       /**< private */
    int      img_data_owner; /**< private */
    int      self_allocd;    /**< private */
} on2_image_t; /**< alias for struct on2_image */

/**\brief Representation of a rectangle on a surface */
typedef struct on2_image_rect {
    unsigned int x; /**< leftmost column */
    unsigned int y; /**< topmost row */
    unsigned int w; /**< width */
    unsigned int h; /**< height */
} on2_image_rect_t; /**< alias for struct on2_image_rect */

/*!\brief Open a descriptor, allocating storage for the underlying image
 *
 * Returns a descriptor for storing an image of the given format. The
 * storage for the descriptor is allocated on the heap.
 *
 * \param[in]    img       Pointer to storage for descriptor. If this parameter
 *                         is NULL, the storage for the descriptor will be
 *                         allocated on the heap.
 * \param[in]    fmt       Format for the image
 * \param[in]    d_w       Width of the image
 * \param[in]    d_h       Height of the image
 * \param[in]    align     Alignment, in bytes, of each row in the image.
 *
 * \return Returns a pointer to the initialized image descriptor. If the img
 *         parameter is non-null, the value of the img parameter will be
 *         returned.
 */
on2_image_t* on2_img_alloc(on2_image_t  *img,
                           img_fmt_t fmt,
                           unsigned int d_w,
                           unsigned int d_h,
                           unsigned int align);

/*!\brief Open a descriptor, using existing storage for the underlying image
 *
 * Returns a descriptor for storing an image of the given format. The
 * storage for descriptor has been allocated elsewhere, and a descriptor is
 * desired to "wrap" that storage.
 *
 * \param[in]    img       Pointer to storage for descriptor. If this parameter
 *                         is NULL, the storage for the descriptor will be
 *                         allocated on the heap.
 * \param[in]    fmt       Format for the image
 * \param[in]    d_w       Width of the image
 * \param[in]    d_h       Height of the image
 * \param[in]    align     Alignment, in bytes, of each row in the image.
 * \param[in]    img_data  Storage to use for the image
 *
 * \return Returns a pointer to the initialized image descriptor. If the img
 *         parameter is non-null, the value of the img parameter will be
 *         returned.
 */
on2_image_t* on2_img_wrap(on2_image_t  *img,
                          img_fmt_t fmt,
                          unsigned int d_w,
                          unsigned int d_h,
                          unsigned int align,
                          unsigned char      *img_data);


/*!\brief Set the rectangle identifying the displayed portion of the image
 *
 * Updates the displayed rectangle (aka viewport) on the image surface to
 * match the specified coordinates and size.
 *
 * \param[in]    img       Image descriptor
 * \param[in]    x         leftmost column
 * \param[in]    y         topmost row
 * \param[in]    w         width
 * \param[in]    h         height
 *
 * \return 0 if the requested rectangle is valid, nonzero otherwise.
 */
int on2_img_set_rect(on2_image_t  *img,
                     unsigned int  x,
                     unsigned int  y,
                     unsigned int  w,
                     unsigned int  h);


/*!\brief Flip the image vertically (top for bottom)
 *
 * Adjusts the image descriptor's pointers and strides to make the image
 * be referenced upside-down.
 *
 * \param[in]    img       Image descriptor
 */
void on2_img_flip(on2_image_t *img);

/*!\brief Close an image descriptor
 *
 * Frees all allocated storage associated with an image descriptor.
 *
 * \param[in]    img       Image descriptor
 */
void on2_img_free(on2_image_t *img);

typedef void (*on2_rgb_to_yuv_t)(
    unsigned char *RGBBuffer,
    int ImageWidth,
    int ImageHeight,
    unsigned char *YBuffer,
    unsigned char *UBuffer,
    unsigned char *VBuffer,
    int SrcPitch,
    int DstPitch);

on2_rgb_to_yuv_t on2_get_rgb_to_yuv(img_fmt_t dst, img_fmt_t src);

#ifdef __cplusplus
}
#endif
#endif
