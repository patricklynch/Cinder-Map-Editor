/**
 Simple function for using built-in 'discard' function to clip fragments outside the bounds specified
 */

void _discardOutsideBounds( vec3 position, vec3 min, vec3 max )
{
	if (position.x < min.x || position.x > max.x ||
		position.y < min.y || position.y > max.y ||
		position.z < min.z || position.z > max.z ) {
		discard;
	}
}