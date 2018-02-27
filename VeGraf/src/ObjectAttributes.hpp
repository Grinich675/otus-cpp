#pragma once

/**
 \brief Common structure, used for color representation
  \details
  Represents color in RGBA style
 */
struct Color
{
	unsigned char r	;	/**<Red color component   */
	unsigned char g;	/**<Green color component */
	unsigned char b;	/**<Blue color component  */
	unsigned char a;	/**<Alpha channel value */
};

/**
 \brief Common object attributes
  \details
  Object-wide attributes
 */
struct Attributes
{
	Color border_color;		/**< Color used for printing borders */
	Color fill;				/**< Color used for space inside an object */
	int border_thickness;	/**< Thickness of borders*/
};

/**
 \brief Common structure, used for coordinates representation
  \details
  Consists only from 2 doubles
 */
struct Coordinates
{
	double x;
	double y;
};
