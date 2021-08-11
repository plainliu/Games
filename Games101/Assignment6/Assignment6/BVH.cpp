#include <algorithm>
#include <cassert>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
                Union(centroidBounds, objects[i]->getBounds().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().x <
                       f2->getBounds().Centroid().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().y <
                       f2->getBounds().Centroid().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().z <
                       f2->getBounds().Centroid().z;
            });
            break;
        }

        auto beginning = objects.begin( );
        auto ending = objects.end( );
        std::vector<Object*>::iterator middling = objects.begin( );

        std::vector<Object*> leftshapes;
        std::vector<Object*> rightshapes;

        if ( splitMethod == SplitMethod::NAIVE || splitMethod == SplitMethod::SAH && objects.size( ) <= 4 )
        {
            middling += ( objects.size( ) / 2 );

            leftshapes = std::vector<Object*>( beginning, middling );
            rightshapes = std::vector<Object*>( middling, ending );
        }
        else if ( splitMethod == SplitMethod::SAH )
        {
            constexpr int nBuckets = 12;
            struct BucketInfo {
                int count = 0;
                Bounds3 bounds;
            };
            BucketInfo buckets[nBuckets];

            for ( auto obj : objects ) {
                const Vector3f centroid = centroidBounds.Offset( obj->getBounds( ).Centroid( ) );
                int b = nBuckets * centroid[dim];
                if ( b == nBuckets ) b = nBuckets - 1;
                buckets[b].count++;
                buckets[b].bounds = Union( buckets[b].bounds, obj->getBounds( ) );
            }

            float cost[nBuckets - 1];
            for ( int i = 0; i < nBuckets - 1; ++i ) {
                Bounds3 b0, b1;
                int count0 = 0, count1 = 0;

                for ( int j = 0; j <= i; ++j ) {
                    b0 = Union( b0, buckets[j].bounds );
                    count0 += buckets[j].count;
                }
                for ( int j = i + 1; j < nBuckets; ++j ) {
                    b1 = Union( b1, buckets[j].bounds );
                    count1 += buckets[j].count;
                }
                cost[i] = 0.125f + ( count0 * b0.SurfaceArea( ) +
                    count1 * b1.SurfaceArea( ) ) / bounds.SurfaceArea( );
            }

            float minCost = cost[0];
            int minCostSplitBucket = 0;
            for ( int i = 1; i < nBuckets - 1; ++i ) {
                if ( cost[i] < minCost ) {
                    minCost = cost[i];
                    minCostSplitBucket = i;
                }
            }

            for ( auto obj : objects ) {
                const Vector3f centroid = centroidBounds.Offset( obj->getBounds( ).Centroid( ) );
                int b = nBuckets * centroid[dim];
                if ( b == nBuckets ) b = nBuckets - 1;
                if ( b <= minCostSplitBucket )
                    middling++;
            }
        }
        else
        {
            std::cout << "[ERR] Invalid SplitMethod: " << (int)splitMethod << std::endl;
            assert( false );
        }

        leftshapes = std::vector<Object*>(beginning, middling);
        rightshapes = std::vector<Object*>(middling, ending);

        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray& ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersection
    bool inter = node->bounds.IntersectP( ray, Vector3f( 1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z ), std::array<int, 3> { ray.direction.x > 0, ray.direction.y > 0, ray.direction.z > 0 } );
    if ( inter == false ) return Intersection( );

    // leaf node
    if ( node->object != nullptr )
       return node->object->getIntersection( ray );

    Intersection left = getIntersection( node->left, ray );
    Intersection right = getIntersection( node->right, ray );

    return left.distance < right.distance ? left : right;
}