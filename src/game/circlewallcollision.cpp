#include "game/hsd_jobj.h"
#include "game/ut.h"
#include "game/gmath.h"
#include "game/edgeperpendicular.h"
extern "C" {
#include "game/cvec.h"
}

extern "C" {
CVec ReflectVectorAboutNormal(const Vec* v, const Vec* normal);
}

// which side of the line ab p is on, looking down from above
static inline f32 LineSide(Vec& a, Vec& b, Vec& p)
{
	return p.x * (a.z - b.z) + a.x * (b.z - p.z) + b.x * (p.z - a.z);
}

// how far p is from the line ab, signed
static inline f32 LineDist(Vec* p, Vec* a, Vec* b)
{
	Vec n;
	CalculateOutwardPerpendicular(p, a, b, &n);
	PSVECNormalize(&n, &n);
	Vec d;
	PSVECSubtract(a, p, &d);
	return PSVECDotProduct(&n, &d);
}

/**
 * @note Address: 0x8018D43C
 * @note Size: 0x1204
 */
// moves a circle of radius from from to to against the wall ab, flat on the xz plane.
// pos gets where it ends up, contact where it touched, dist how far the start is from the
// wall and push what's left of the move. returns 0 for no hit, 1 when it got stopped
// and 3 when it bounced, then push is the move reflected off the wall times scale
// the wall's two ends get checked like posts, corners picks the softer version of that
#pragma section RX ".text.circlewall"
extern "C" __declspec(section ".text.circlewall") int ResolveCircleWallCollisionXZ(const Vec* fromPos, const Vec* toPos, const Vec* wallA,
                                                                                   const Vec* wallB, Vec* push, Vec* pos, Vec* contact,
                                                                                   f32* dist, f32 radius, f32 scale, int corners)
{
	CVec from(*fromPos);
	from.y = 0.0f;
	CVec to(*toPos);
	to.y     = 0.0f;
	f32 len  = (to - from).Length();
	CVec dir = (to - from).Normalize();
	CVec a(*wallA);
	a.y = 0.0f;
	CVec b(*wallB);
	b.y = 0.0f;
	CVec n;
	CalculateOutwardPerpendicular(&from, &a, &b, &n);
	PSVECNormalize(&n, &n);
	CVec fromEdge = from + n * radius; // never used, it's still computed
	CVec toEdge   = to + n * radius;
	int ret       = 0;

	if (LineSide(a, b, toEdge) >= 0.0f && LineSide(a, b, from) <= 0.0f) {
		CVec v;
		CVec hit;
		if (IntersectLinesXZ(&from, &toEdge, &a, &b, &hit, 1)) {
			if (contact) {
				*contact = hit;
			}
			if (dist && *dist < 0.0f) {
				*dist = LineDist(&from, &a, &b);
			}
			CVec end;
			IntersectLinesXZ(&toEdge, &to, &a, &b, &end, 0);
			*pos   = end - n * radius;
			pos->y = fromPos->y;
			if (dist && ABS(*dist - LineDist(&from, &a, &b)) < 0.1f) {
				push->x = 0.0f;
				push->y = 0.0f;
				push->z = 0.0f;
				ret     = 1;
			} else {
				*push = ReflectVectorAboutNormal(&dir, &n) * len * scale;
				ret   = 3;
			}
		} else if (corners) {
			if (PSVECDistance(&to, &a) <= radius) {
				if (contact) {
					*contact = a;
				}
				v      = (to - a).Normalize();
				*pos   = a + v * radius + (to - from);
				pos->y = fromPos->y;
				*push  = len > 0.3f ? to - from : CVec(0.0f, 0.0f, 0.0f);
				ret    = 1;
			}
			if (PSVECDistance(&to, &b) <= radius) {
				if (contact) {
					*contact = b;
				}
				v      = (to - b).Normalize();
				*pos   = b + v * radius + (to - from);
				pos->y = fromPos->y;
				*push  = len > 0.3f ? to - from : CVec(0.0f, 0.0f, 0.0f);
				ret    = 1;
			}
		} else {
			if (PSVECDistance(&to, &a) < radius) {
				if (contact) {
					*contact = a;
				}
				v      = (to - a).Normalize();
				*pos   = a + v * radius * 1.001f;
				pos->y = fromPos->y;
				if (PSVECDistance(&from, &a) >= 0.01f + radius) {
					*push = v * len * scale;
					ret   = 3;
				} else {
					IntersectLinesXZ(&from, &toEdge, &a, &b, &hit, 0);
					CVec e = hit + dir * len;
					CVec f = e - n;
					CVec g;
					IntersectLinesXZ(&e, &f, &a, &b, &g, 0);
					v = g - hit;
					if (v.Length() > len) {
						*push = (g - hit).Normalize() * len;
					} else {
						*push = v;
					}
					ret = 1;
				}
			} else if (PSVECDistance(&to, &b) < radius) {
				if (contact) {
					*contact = b;
				}
				v      = (to - b).Normalize();
				*pos   = b + v * radius * 1.001f;
				pos->y = fromPos->y;
				if (PSVECDistance(&from, &b) >= 0.01f + radius) {
					*push = v * len * scale;
					ret   = 3;
				} else {
					IntersectLinesXZ(&from, &toEdge, &a, &b, &hit, 0);
					CVec e = hit + dir * len;
					CVec f = e - n;
					CVec g;
					IntersectLinesXZ(&e, &f, &a, &b, &g, 0);
					v = g - hit;
					if (v.Length() > len) {
						*push = (g - hit).Normalize() * len;
					} else {
						*push = v;
					}
					ret = 1;
				}
			}
		}
	}
	if (dist) {
		*dist = LineDist(&from, &a, &b);
	}
	return ret;
}
