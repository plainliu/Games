//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm hereshade(p, wo)
    if ( depth > this->maxDepth )
        return Vector3f( 0.0, 0.0, 0.0 );

    Intersection p = Scene::intersect( ray );
    if ( p.happened == false )
        return Vector3f(0.f);

    Material* m = p.m;
    Vector3f P = p.coords;
    Vector3f N = p.normal;

    // Test Normal
    //hitColor = (N + 1.0f) * 0.5f;

    Vector3f lightdir(0.f);

    Intersection interLight;
    float pdf_light = 0.0f;
    //sampleLight( inter, pdf_light )
    //Get x, ws, NN, emit from inter
    sampleLight( interLight, pdf_light );
    //Shoot a ray from p to x
    Vector3f ws = ( interLight.coords - P ).normalized( );
    Ray r( P, ws );
    //If the ray is not blocked in the middle
    Intersection hitLight = intersect( r );
    if ( hitLight.obj && hitLight.obj->hasEmit( ) == true )
    {
        //L_dir = emit * eval( wo, ws, N ) * dot( ws, N ) * dot( ws,
        //    NN ) / |x - p | ^ 2 / pdf_light
        lightdir = interLight.emit * m->eval( -ray.direction, ws, N )
            * dotProduct( ws, N) * dotProduct( -ws, interLight.normal)
            / pow( ( P - interLight.coords ).norm(), 2 ) / pdf_light;
    }

    //L_indir = 0.0
    Vector3f L_indir( 0.f );

    //Test Russian Roulette with probability RussianRoulette
    //wi = sample( wo, N )
    Vector3f wi = m->sample( ray.direction, N );
    //Trace a ray r( p, wi )
    Intersection hitObj = intersect( Ray( P, wi ));
    //If ray r hit a non - emitting object at q
    if ( hitObj.happened && hitObj.obj->hasEmit( ) == false )
    {
        //L_indir = shade( q, wi ) * eval( wo, wi, N ) * dot( wi, N )
        // / pdf( wo, wi, N ) / RussianRoulette
        L_indir = castRay( Ray( P, wi ), 0) * m->eval( -ray.direction, wi, N )
            * dotProduct( wi, N ) / m->pdf( -ray.direction, wi, N ) / RussianRoulette;
    }

    //Return L_dir + L_indir
    return lightdir + L_indir + m->getEmission( );
}