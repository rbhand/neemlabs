//
//  types.h
//  spritesheetgen
//
//  Created by Vitaliy Ivanov on 6/26/12.
//

#pragma once

namespace ssg
{
	struct size_t
	{
		int width, height;
	};
	
	struct point_t
	{
		int x, y;
	};
	
	struct rect_t
	{
		rect_t() :
			x(0), y(0), width(0), height(0) { }
			
		rect_t(int ax, int ay, int awidth, int aheight) :
			x(ax), y(ay), width(awidth), height(aheight) { }
		
		bool isNull() const
		{
			return width == 0 || height == 0;
		}
		
		int x1() const { return x; }
		int y1() const { return y; }
		int x2() const { return x + width - 1; }
		int y2() const { return y + height - 1; }
		
		bool intersects(const rect_t& r) const
		{
		    if (isNull() || r.isNull())
				return false;
			
			int l1 = x1();
			int r1 = x1();
			if (x2() - x1() + 1 < 0)
				l1 = x2();
			else
				r1 = x2();

			int l2 = r.x1();
			int r2 = r.x1();
			if (r.x2() - r.x1() + 1 < 0)
				l2 = r.x2();
			else
				r2 = r.x2();

			if (l1 > r2 || l2 > r1)
				return false;

			int t1 = y1();
			int b1 = y1();
			if (y2() - y1() + 1 < 0)
				t1 = y2();
			else
				b1 = y2();

			int t2 = r.y1();
			int b2 = r.y1();
			if (r.y2() - r.y1() + 1 < 0)
				t2 = r.y2();
			else
				b2 = r.y2();

			if (t1 > b2 || t2 > b1)
				return false;

			return true;
		}
		
		rect_t united(const rect_t& r)
		{
			if (isNull())
				return r;
			if (r.isNull())
				return *this;

			int l1 = x1();
			int r1 = x1();
			if (x2() - x1() + 1 < 0)
				l1 = x2();
			else
				r1 = x2();

			int l2 = r.x1();
			int r2 = r.x1();
			if (r.x2() - r.x1() + 1 < 0)
				l2 = r.x2();
			else
				r2 = r.x2();

			int t1 = y1();
			int b1 = y1();
			if (y2() - y1() + 1 < 0)
				t1 = y2();
			else
				b1 = y2();

			int t2 = r.y1();
			int b2 = r.y1();
			if (r.y2() - r.y1() + 1 < 0)
				t2 = r.y2();
			else
				b2 = r.y2();

			rect_t tmp;
			tmp.x = std::min(l1, l2);
			tmp.width = std::max(r1, r2) - tmp.x + 1;
			tmp.y = std::min(t1, t2);
			tmp.height = std::max(b1, b2) - tmp.y + 1;
			return tmp;
		}
		
		
		int x, y, width, height;
	};
}